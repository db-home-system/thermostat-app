#include "appconfig.h"
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
#include <QFileInfo>

#include <QtDebug>


Thermostat::Thermostat(QObject *parent) : QObject(parent),
    _watcher(new QFileSystemWatcher(this))
{
    // Load configuration
    _cfg = AppConfig::instance();

    connect(_watcher, &QFileSystemWatcher::fileChanged,
            this, &Thermostat::fileSettingsChanged);
    connect(_watcher, &QFileSystemWatcher::directoryChanged,
            this, &Thermostat::dirSettingsChanged);


    // Set correct size for timelines slots.
    timeline_slots.resize(TIMELINE_DIVISION);
    for (int i = 0; i < TIMELINE_DIVISION; i++) {
        timeline_slots[i].onOff = 0;
        timeline_slots[i].tempSP = 15.0;
    }

    _current_hour = 0;

    // Register timer for pid controll
    _pid_timer = new QTimer(this);
    connect(_pid_timer, &QTimer::timeout, this, &Thermostat::pidControll);
    _pid_timer->start(DEFAULT_PID_TICK);

    _status = 0;
    _int_temp = NOTEMP;
    _ext_temp = NOTEMP;

    _sens_type_map = {
        {"intTemp", INT_TEMP},
        {"extTemp", EXT_TEMP},
    };

    // Trigger first time the timeline setting configuration
    QTimer::singleShot(100, this, &Thermostat::dirSettingsChanged);
    QTimer::singleShot(100, this, &Thermostat::fileSettingsChanged);
}

void Thermostat::updateHour(int h)
{
    _current_hour = h;
}

void Thermostat::dirSettingsChanged()
{
    _watcher->addPath(_cfg->settingDir());
    _watcher->addPath(_cfg->timelineFile());
    qDebug() << "settings" << _watcher->files() << _watcher->directories();
}

void Thermostat::fileSettingsChanged()
{
    QList<QStringList> sett;
    if (readCSVFile(_cfg->timelineFile(), sett))
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

    writeLineToFile(_cfg->pidOutFile(), outpid);
}

void Thermostat::pidControll()
{

    readSensData();

    QMapIterator<int, SensMap> i(_sensors_data);
    int avg_ext = NOTEMP;
    int avg_int = NOTEMP;
    int c_ext = 0;
    int c_int = 0;
    while (i.hasNext())
    {
        i.next();
        SensMap m = i.value();

        if (m.data == NOTEMP)
            continue;

        if (m.type == INT_TEMP && m.update)
        {
            if (avg_int == NOTEMP)
                avg_int = 0;

            avg_int += m.data;
            c_int++;
            m.update = false;
        }

        if (m.type == EXT_TEMP && m.update)
        {
            if (avg_ext == NOTEMP)
                avg_ext = 0;

            avg_ext += m.data;
            c_ext++;
            m.update = false;
        }
    }

    if (c_ext == 0)
        c_ext = 1;

    if (c_int == 0)
        c_int = 1;

    _ext_temp = avg_ext/c_ext;
    _int_temp = avg_int/c_int;
    _dev_temp = readDeviceTemperature();

//    qDebug() << _ext_temp << _int_temp << _dev_temp;

    _processed_temp = _dev_temp;
    if (_int_temp != NOTEMP)
        _processed_temp = (_dev_temp + _int_temp) / 2;

    int sp = timeline_slots[_current_hour].tempSP;
    int onoff = 0;
    if (timeline_slots[_current_hour].onOff)
    {
      if (_processed_temp < sp)
          onoff = 1;
    }

    dump(sp);
    heaterOnOff(onoff);

    int tick = 0;
    if (_cfg->appMode() == AppConfig::TEST)
        tick = _cfg->simPIDTick();

    _pid_timer->setInterval(tick);

    emit statusChanged();
}

void Thermostat::heaterOnOff(int cmd)
{
    if (writeLineToFile(_cfg->statusFile(), QString::number(cmd)))
        _status = cmd;
}


int Thermostat::readDeviceTemperature()
{
    QString t = readLineFromFile(_cfg->tempDevice());
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
    if (readCSVFile(_cfg->sensorDevice(), data))
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
            sens.update = false;

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
            sens.data = d; //static_cast<int>(d * 1000);
            sens.desc = data[i][3];
            sens.update = true;

            _sensors_data[index] = sens;
        }
    }
}

