#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "manager.h"

#include <QObject>
#include <QVector>

class QFileSystemWatcher;
class AppConfig;
class QTimer;

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
    enum SensTypes {
        INT_TEMP,
        EXT_TEMP,

        ERR_TYPE
    };

    struct SensMap {
        int data;
        SensTypes type;
        QString desc;
        bool update;
    };

    QMap<QString, SensTypes> _sens_type_map;

    void dump(float sp);
    void pidControll();

    bool loadTimelineCfg(QString cfg, QList<QStringList> &l);

    int readDeviceTemperature();

    void readSensData();
    void inputSensDataDirChanged();

    void heaterOnOff(int cmd);

    QFileSystemWatcher * const _watcher;
    QFileSystemWatcher * const _watcher_sensors;
    QVector<TimelineSlotsData> timeline_slots;

    int _current_hour;

    // Status of thermostatat
    int _status;
    int _dev_temp;
    int _int_temp;
    int _ext_temp;
    int _processed_temp;

    QMap<int, SensMap> _sensors_data;

    AppConfig *_cfg;
    QTimer *_pid_timer;

};

#endif // THERMOSTAT_H
