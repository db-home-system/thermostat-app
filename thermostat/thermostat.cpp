#include "app-config.h"
#include "manager.h"
#include "thermostat.h"

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
}

void Thermostat::dirSettingsChanged()
{
    // root path for all settings
    _root_path = APP_SETTINGS_ROOT;
    QByteArray qroot_path = qgetenv(APP_SETTINGS_ROOT_ENV);
    if (!qroot_path.isEmpty())
        _root_path = QString::fromLocal8Bit(qroot_path);

    _settings_path = _root_path + TIMELINE_CFG;
    QByteArray qsettings_path = qgetenv(TIMELINE_CFG_ENV);
    if (!qsettings_path.isEmpty())
        _settings_path = QString::fromLocal8Bit(qsettings_path);

    qDebug() << _root_path << _settings_path;

    _watcher->addPath(_root_path);
    _watcher->addPath(_settings_path);
    qDebug() << _watcher->files() << _watcher->directories();
}

void Thermostat::fileSettingsChanged()
{
    qDebug() << "Read from: " << _settings_path;

    QList<QStringList> sett;
    if (loadTimelineCfg(_settings_path, sett))
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
        emit dataChanged(&timeline_slots);
    }
}

void Thermostat::pidControll()
{
    QTime now = QTime::currentTime();
    current_hour = now.hour();

    float sp = timeline_slots[current_hour].tempSP;
    qDebug() << "pid " << sp;

    float temp_dev  =  readDeviceTemperature();
    float temp_sens =  readSensorTemperature();
    float temp_ext  =  readSensorExtTemperature();

    float processed_temp = 0.0;

    qDebug() << "dev: " << temp_dev << "sen: " << temp_sens << "ext: " << temp_ext;

    bool onoff = false;
    if (processed_temp < sp)
        onoff = true;

    heaterOnOff(onoff);

}

void Thermostat::heaterOnOff(bool cmd)
{
    qDebug() << "Heater " << cmd;

    emit heaterStatusChanged(cmd);
}

bool Thermostat::loadTimelineCfg(QString cfg, QList<QStringList> &l)
{
    QFile file(cfg);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return false;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        QString lline = QString::fromLocal8Bit(line);
        l.append(lline.split(';'));
        qDebug() << lline;
    }
    file.close();

    return true;
}

float Thermostat::readDeviceTemperature()
{
    return qrand() % ((22 + 1) - 18) + 18;
}

float Thermostat::readSensorExtTemperature()
{
    float temp = 0.0;
    for (int i = 0; i < 12; i++) {
        temp += (qrand() % ((22 + 1) - 18) + 18);
    }

    return temp / 12;
}

float Thermostat::readSensorTemperature()
{
    float temp = 0.0;
    for (int i = 0; i < 12; i++) {
        temp += (qrand() % ((22 + 1) - 18) + 18);
    }

    return temp / 12;
}
