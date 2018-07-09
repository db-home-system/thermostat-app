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
    _int_temp = NOTEMP;
    _ext_temp = NOTEMP;

    // root path for all settings
    _input_root_path = inputsRootPath();

    // Trigger first time the timeline setting configuration
    QTimer::singleShot(100, this, &Thermostat::fileSettingsChanged);


    _sens_type_map = {
        {"intTemp", INT_TEMP},
        {"extTemp", EXT_TEMP},
    };
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
            if ((slot_temp > 40000 || slot_temp < 0) || !(check)) {
                qDebug() << "Invalid temperature." << slot_temp;
                continue;
            }

            timeline_slots[h].onOff = on_off;
            timeline_slots[h].tempSP = slot_temp;
        }
        emit dataChanged(timeline_slots);
    }
}
void Thermostat::dump(float sp)
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
    outpid += QString::number(_processed_temp);
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

        if (m.data == NOTEMP)
            continue;

        if (m.type == INT_TEMP && m.data != _int_temp)
        {
            if (_int_temp == NOTEMP)
            {
                _int_temp = m.data;
            }
            else
            {
                _int_temp = (_int_temp + m.data) / 2;
            }
        }

        if (m.type == EXT_TEMP && m.data != _ext_temp)
        {
            if (_ext_temp == NOTEMP)
            {
                _ext_temp = m.data;
            }
            else
            {
                _ext_temp = (_ext_temp + m.data) / 2;
            }
        }
    }

    _dev_temp = readDeviceTemperature();

    _processed_temp = _dev_temp;
    if (_int_temp != NOTEMP)
        _processed_temp = (_dev_temp + _int_temp) / 2;

    int onoff = 0;
    if ((sp - _processed_temp) >= 5)
        onoff = 1;

    dump(sp);

    heaterOnOff(onoff);
    emit statusChanged();
}

void Thermostat::heaterOnOff(int cmd)
{
    QString out_file = outputRootPath() + "heater";
    if (writeLineToFile(out_file, QString::number(cmd)))
        _status = cmd;
}


int Thermostat::readDeviceTemperature()
{
    QString t = readLineFromFile(_input_root_path + "device_temp");
    if (t.isNull())
        return NOTEMP;

    bool ok = false;
    int d = t.toInt(&ok);
    if (!ok)
    {
        qDebug() << "Unable to convert data" << t;
        return NOTEMP;
    }

    return d; //static_cast<int>(d * 1000);
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

            bool check = false;
            SensMap sens;
            sens.data = NOTEMP;

            int index = data[i][0].toInt(&check);
            if (!check)
                continue;

            sens.type = _sens_type_map.value(data[i][1], ERR_TYPE);

            float d = data[i][2].toFloat(&check);
            if (!check) {
                qDebug() << "Unable to convert data" << sens.data;
                continue;
            }

            // Convert in mdegCelsius
            sens.data = static_cast<int>(d * 1000);
            sens.desc = data[i][3];

            _sensors_data[index] = sens;
        }
    }
}

