#include "app-config.h"
#include "manager.h"
#include "thermostat.h"
#include "utils.h"

#include <QVector>
#include <QTimer>
#include <QFile>
#include <QTime>
#include <QFileSystemWatcher>

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

    // Register timer for pid controll
    QTimer *pid = new QTimer(this);
    connect(pid, &QTimer::timeout, this, &Thermostat::pidControll);
    pid->start(1000);

    // Trigger first time the timeline setting configuration
    QTimer::singleShot(100, this, &Thermostat::fileSettingsChanged);

    _status = 0;
    _int_temp = 0;
    _ext_temp = 0;

    // root path for all settings
    _root_path = settingsRootPath();
    _settings_path =  settingsPath();
    _input_root_path = inputsRootPath();
}

void Thermostat::dirSettingsChanged()
{
    qDebug() << _root_path << _settings_path;

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

void Thermostat::pidControll()
{
    QTime now = QTime::currentTime();
    _current_hour = now.hour();
    float sp = timeline_slots[_current_hour].tempSP;
    qDebug() << "pid " << sp;

    readSensData();
    for (int i = 0; i < _sensors_data.size(); i++)
    {
        SensMap m = _sensors_data[i];

        if (m.type == "intTemp" && m.data != _int_temp)
            _int_temp = (_int_temp + m.data) / 2;

        if (m.type == "extTemp" && m.data != _ext_temp)
            _ext_temp = (_ext_temp + m.data) / 2;
    }

    _dev_temp = readDeviceTemperature();

    qDebug() << "intTemp:" << _int_temp << "extTemp:" << _ext_temp;
    qDebug() << "devTemp:" << _dev_temp;

    float processed_temp = 0.0;
    bool onoff = false;
    if (processed_temp < sp)
        onoff = true;

    heaterOnOff(onoff);

    emit statusChanged();
}

void Thermostat::heaterOnOff(bool cmd)
{
    qDebug() << "Heater " << cmd;
    _status = cmd;
}


float Thermostat::readDeviceTemperature()
{
    QString t = readLineFromFile(_input_root_path + "device_temp.cfg");
    if (t == "")
        return -273.0;

    return t.toFloat();
}

void Thermostat::readSensData()
{
    QList<QStringList> data;
    if (readCSVFile(_input_root_path + "sensor_data.cfg", data))
    {
        for (int i = 0; i < data.size(); i++)
        {
            if (data[i].size() < 4) {
                qDebug() << "Ext sens o enought elements to unpack.";
                continue;
            }

            SensMap sens;
            bool check = false;

            sens.index = data[i][0].toInt(&check);
            if (!check)
                sens.index = -1;

            sens.type = data[i][1];

            sens.data = data[i][2].toFloat(&check);
            if (!check) {
                qDebug() << "Unable to convert data" << sens.data;
                continue;
            }

            sens.desc = data[i][3];
            _sensors_data.append(sens);
        }
    }
}

