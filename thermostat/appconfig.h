#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QMap>

// Timeline cfg
#define TIMELINE_DIVISION         24
#define NOTEMP                    -273000      // mdegCelsius
#define NOPRESSURE                -1
#define NOHUMIDITY                -1
#define NOICON                    "images/icons/00d.svg"
#define DEFAULT_TIMELINE_LOWTEMP  10000        // mdegCelsius

#define DEFAULT_PID_TICK     1000 // millisecons
#define DEFAULT_CLOCK_TICK   1000 // millisecons

class QSettings;

class AppConfig : public QObject
{
    Q_OBJECT

public:
    enum AppModeType {
        TEST,

        MODE_ERR
    };

    QMap<QString, AppModeType> _appmode_map;
    static AppConfig *instance(QObject *parent = nullptr);

    QString timelineFile();
    QString tempDevice();
    QString sensorDevice();
    QString pidOutFile();
    QString statusFile();
    QString modeFile();
    QString runDir()     const { return _root_run_path; }
    QString settingDir() const { return _root_setting_path; }
    QString inputDir()   const { return _root_intput_path; }

    AppModeType appMode();
    QString getSimTimeClock();
    int simTick();
    int simPIDTick();
    QString owmToken();
    QString owmURL();
    QString owmPlace();

signals:

public slots:

private:
    AppConfig(QObject *parent = nullptr);

    QSettings *_cfg;
    QString _root_run_path;
    QString _root_setting_path;
    QString _root_intput_path;
};

#endif // APPCONFIG_H
