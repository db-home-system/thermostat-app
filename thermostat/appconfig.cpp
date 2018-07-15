#include "appconfig.h"
#include "utils.h"

#include <QSettings>

#include <QDir>

#include <QDebug>

#define DEFAULT_RUN_DIR  "/var/lib/thermoapp/run/"
#define DEFAULT_MODE      "mode.out"
#define DEFAULT_SIMTICK   "sim_clock.in"
#define DEFAULT_PID       "pid.out"
#define DEFAULT_STATUS    "status.log"

#define DEFAULT_SETTING_DIR  "/var/lib/thermoapp/settings/"
#define DEFAULT_TIMELINE     "timeline.in"

#define DEFAULT_TEMP      "/var/lib/thermoapp/input/onboard_temp"
#define DEFAULT_SENS      "/var/lib/thermoapp/input/sensor_data"

AppConfig::AppModeType AppConfig::appMode()
{
    if (_root_run_path.isNull() || _root_run_path.isEmpty())
    {
        qDebug() << "Run root path is empty!";
        return MODE_ERR;
    }

    QByteArray m = _cfg->value("mode_file", DEFAULT_MODE).toByteArray();
    QString ll = readLineFromFile(QDir(_root_run_path).filePath(m));
    if (ll.isNull() || ll.isEmpty())
        return MODE_ERR;

    return _appmode_map.value("error", MODE_ERR);
}

QString AppConfig::getSimTimeClock()
{
    QByteArray m = _cfg->value("time_clock_file", DEFAULT_SIMTICK).toByteArray();
    QString p = QDir(_root_run_path).filePath(m);
    QString ll = readLineFromFile(p);
    if (ll.isNull() || ll.isEmpty())
        return "00:00:00";

    return ll;
}

int AppConfig::simTick()
{
    int s = _cfg->value("sim_tick", 1000).toInt();
    return s;
}

int AppConfig::simPIDTick()
{
    return _cfg->value("sim_pid_tick", 100).toInt();
}

QString AppConfig::timelineFile()
{
    QByteArray s = _cfg->value("timeline_file", DEFAULT_TIMELINE).toByteArray();
    QString p = QDir(_root_setting_path).filePath(QString::fromLocal8Bit(s));
    return p;
}

QString AppConfig::tempDevice()
{
    QByteArray s = _cfg->value("onboard_temp", DEFAULT_TEMP).toByteArray();
    return s;
}

QString AppConfig::sensorDevice()
{
    QByteArray s = _cfg->value("sensor_input", DEFAULT_SENS).toByteArray();
    return QString::fromLocal8Bit(s);
}

QString AppConfig::pidOutFile()
{
    QByteArray s = _cfg->value("pid_log", DEFAULT_PID).toByteArray();
    QString p = QDir(_root_run_path).filePath(QString::fromLocal8Bit(s));
    return p;
}

QString AppConfig::statusFile()
{
    QByteArray s = _cfg->value("status_file", DEFAULT_STATUS).toByteArray();
    QString p = QDir(_root_run_path).filePath(QString::fromLocal8Bit(s));
    return p;
}

QString AppConfig::modeFile()
{
    QByteArray s = _cfg->value("mode_file", DEFAULT_MODE).toByteArray();
    QString p = QDir(_root_run_path).filePath(QString::fromLocal8Bit(s));
    return p;
}

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    QString setting_file = "/etc/thermapp/thermapp.conf";
    QByteArray mode = qgetenv("THERMAPP_CONF");
    if (!mode.isEmpty())
        setting_file = QString::fromLocal8Bit(mode);

    _cfg = new QSettings(setting_file, QSettings::IniFormat, parent);

    QByteArray m = _cfg->value("run_directory", DEFAULT_RUN_DIR).toByteArray();
    _root_run_path = QString::fromLocal8Bit(m);

     m = _cfg->value("setting_directory", DEFAULT_SETTING_DIR).toByteArray();
    _root_setting_path = QString::fromLocal8Bit(m);

    qDebug() << _root_run_path;
    qDebug() << _root_setting_path;

    _appmode_map = {
        {"test",          TEST},
        {"error",     MODE_ERR}
    };
}

AppConfig *AppConfig::instance(QObject *parent)
{
    static AppConfig *appconfig_instance = new AppConfig(parent);

    return appconfig_instance;
}


