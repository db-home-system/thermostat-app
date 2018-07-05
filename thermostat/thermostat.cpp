#include "app-config.h"
#include "manager.h"
#include "thermostat.h"
#include "utils.h"
#include "manager.h"

#include <QVector>
#include <QTimer>
#include <QFile>
#include <QTime>
#include <QFileSystemWatcher>
#include <QtMath>

#include <QtDebug>

Thermostat::Thermostat(QObject *parent) : QObject(parent),
    _watcher(new QFileSystemWatcher(this))
{

    connect(_watcher, &QFileSystemWatcher::fileChanged,
                      this, &Thermostat::fileSettingsChanged);
    connect(_watcher, &QFileSystemWatcher::directoryChanged,
                      this, &Thermostat::dirSettingsChanged);

    dirSettingsChanged();
    // Set correct size for timelines slots.
    timeline_slots.resize(TIMELINE_DIVISION);
    for (int i = 0; i < TIMELINE_DIVISION; i++) {
        timeline_slots[i].onOff = 0;
        timeline_slots[i].tempSP = 15.0;
    }

    _current_hour = 0;

    int tick = 1000;
    if (isSimMode())
        tick = simPIDTick();

    // Register timer for pid controll
    QTimer *pid = new QTimer(this);
    connect(pid, &QTimer::timeout, this, &Thermostat::pidControll);
    pid->start(tick);

    _status = 0;
    _int_temp = -273.0;
    _ext_temp = -273.0;

    // root path for all settings
    _input_root_path = inputsRootPath();

    // Trigger first time the timeline setting configuration
    QTimer::singleShot(100, this, &Thermostat::fileSettingsChanged);
}

void Thermostat::updateHour(int h)
{
    _current_hour = h;
}

void Thermostat::dirSettingsChanged()
{
    _root_path = settingsRootPath();
    _settings_path =  settingsPath();

    _watcher->addPath(_root_path);
    _watcher->addPath(_settings_path);
    qDebug() << _watcher->files() << _watcher->directories();
}

void Thermostat::fileSettingsChanged()
{
    qDebug() << "Read from: " << _settings_path;

    QList<QStringList> sett;
    if (readCSVFile(_settings_path, sett))
    {
        for (int row = 0; row < sett.size(); row++)
        {
            if (sett[row].size() < 3) {
                qDebug() << "no enought elements to unpack.";
                continue;
            }

            bool check = false;
            int h = sett[row][0].toInt(&check);
            if ((h >= 24) || !(check)) {
                qDebug() << "Invalid hour." << h;
                continue;
            }

            check = false;
            int on_off = sett[row][1].toInt(&check);
            if ((on_off != 0 && on_off != 1) || !(check)) {
                qDebug() << "Invalid on off value shoul be 0 or 1" << on_off;
                continue;
            }

            check = false;
            float slot_temp = sett[row][2].toFloat(&check);
            if ((slot_temp > 40.0 || slot_temp < 0.0) || !(check)) {
                qDebug() << "Invalid temperature." << slot_temp;
                continue;
            }

            timeline_slots[h].onOff = on_off;
            timeline_slots[h].tempSP = slot_temp;
        }
        emit dataChanged(timeline_slots);
    }
}
void Thermostat::dump(float sp, float processed_temp)
{
    // Dump to file current pid data
    QString outpid = "#h;devTemp;intTemp;extTemp;Sp;Pt;Status;\n";
    outpid += QString::number(_current_hour);
    outpid += ";";
    outpid += QString::number(_dev_temp);
    outpid += ";";
    outpid += QString::number(_int_temp);
    outpid += ";";
    outpid += QString::number(_ext_temp);
    outpid += ";";
    outpid += QString::number(sp);
    outpid += ";";
    outpid += QString::number(processed_temp);
    outpid += ";";
    outpid += QString::number(_status);
    outpid += ";";

    QString out_file = outputRootPath() + "pid.log";
    writeLineToFile(out_file, outpid);
}

void Thermostat::pidControll()
{
    float sp = timeline_slots[_current_hour].tempSP;
    //qDebug() << sp << _current_hour;

    readSensData();

    QMapIterator<int, SensMap> i(_sensors_data);
     while (i.hasNext())
     {
         i.next();
         SensMap m = i.value();

        if (m.data == -273.0)
            continue;

        if (m.type == "intTemp" && m.data != _int_temp)
        {
            if (_int_temp == -273.0)
                _int_temp = m.data;
            else
                _int_temp = (_int_temp + m.data) / 2;
        }

        if (m.type == "extTemp" && m.data != _ext_temp)
            if (_ext_temp == -273.0)
                _ext_temp = m.data;
            else
                _ext_temp = (_ext_temp + m.data) / 2;
    }

    _dev_temp = readDeviceTemperature();

    // Round with 2 decimal
    _int_temp = static_cast<float>(static_cast<int>(_int_temp*100+0.5))/100.0;
    _ext_temp = static_cast<float>(static_cast<int>(_ext_temp*100+0.5))/100.0;
    _dev_temp = static_cast<float>(static_cast<int>(_dev_temp*100+0.5))/100.0;

    float processed_temp = _dev_temp;
    if (_int_temp != -273.0)
        processed_temp = (_dev_temp + _int_temp) / 2;

    int onoff = 0;
    if ((sp - processed_temp) >= 0.5)
        onoff = 1;

    dump(sp, processed_temp);

    heaterOnOff(onoff);
    emit statusChanged();
}

void Thermostat::heaterOnOff(int cmd)
{
    QString out_file = outputRootPath() + "heater";
    if (writeLineToFile(out_file, QString::number(cmd)))
        _status = cmd;
}


float Thermostat::readDeviceTemperature()
{
    QString t = readLineFromFile(_input_root_path + "device_temp");
    if (t == "")
        return -273.0;

    return t.toFloat();
}

void Thermostat::readSensData()
{
    QList<QStringList> data;
    if (readCSVFile(_input_root_path + "sensor_data", data))
    {
        for (int i = 0; i < data.size(); i++)
        {
            if (data[i].size() < 4) {
                qDebug() << "Ext sens o enought elements to unpack.";
                continue;
            }

            SensMap sens;
            bool check = false;

            int index = data[i][0].toInt(&check);
            if (!check)
                continue;

            sens.type = data[i][1];

            sens.data = data[i][2].toFloat(&check);
            if (!check) {
                qDebug() << "Unable to convert data" << sens.data;
                continue;
            }

            sens.desc = data[i][3];

            _sensors_data[index] = sens;
        }
    }
}

