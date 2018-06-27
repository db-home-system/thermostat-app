#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "manager.h"

#include <QObject>
#include <QVector>

class QFileSystemWatcher;

class Thermostat : public QObject
{
    Q_OBJECT

public:
    explicit Thermostat(QObject *parent = nullptr);

    int status()    { return _status;   }
    float intTemp() { return _int_temp; }
    float extTemp() { return _ext_temp; }

public slots:
    void fileSettingsChanged();
    void dirSettingsChanged();

signals:
    void dataChanged(QVector<TimelineSlotsData> *timeline);
    void statusChanged();

private:
    void pidControll();

    bool loadTimelineCfg(QString cfg, QList<QStringList> &l);

    float readDeviceTemperature();
    float readSensorTemperature();
    float readSensorExtTemperature();

    void heaterOnOff(bool cmd);

    QFileSystemWatcher * const _watcher;
    QString _root_path;
    QString _settings_path;


    QVector<TimelineSlotsData> timeline_slots;

    int _current_hour;

    // Status of thermostatat
    int _status;
    float _int_temp;
    float _ext_temp;
};

#endif // THERMOSTAT_H
