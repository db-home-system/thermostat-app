#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "manager.h"

#include <QObject>
#include <QVector>

struct SensMap {
    int data;
    QString type;
    QString desc;
};

class QFileSystemWatcher;

class Thermostat : public QObject
{
    Q_OBJECT

public:
    explicit Thermostat(QObject *parent = nullptr);

    int status()  const { return _status;         }
    int intTemp() const { return _processed_temp; }
    int extTemp() const { return _ext_temp;       }

public slots:
    void fileSettingsChanged();
    void dirSettingsChanged();
    void updateHour(int h);

signals:
    void dataChanged(QVector<TimelineSlotsData> &timeline);
    void statusChanged();

private:
    void dump(float sp);
    void pidControll();

    bool loadTimelineCfg(QString cfg, QList<QStringList> &l);

    int readDeviceTemperature();
    void readSensData();

    void heaterOnOff(int cmd);

    QFileSystemWatcher * const _watcher;
    QString _root_path;
    QString _settings_path;


    QVector<TimelineSlotsData> timeline_slots;

    int _current_hour;

    // Status of thermostatat
    int _status;
    int _dev_temp;
    int _int_temp;
    int _ext_temp;
    int _processed_temp;

    QString _input_root_path;
    QMap<int, SensMap> _sensors_data;

};

#endif // THERMOSTAT_H
