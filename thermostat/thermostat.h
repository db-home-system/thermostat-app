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

public slots:
    void fileSettingsChanged();
    void dirSettingsChanged();

signals:
    void dataChanged(QVector<TimelineSlotsData> *timeline);
    void heaterStatusChanged(bool status);

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

    int current_hour;
};

#endif // THERMOSTAT_H
