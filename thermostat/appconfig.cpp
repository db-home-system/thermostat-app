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

#define DEFAULT_INPUT_DIR  "/var/lib/thermoapp/input/"
#define DEFAULT_TEMP       "onboard_temp.log"
#define DEFAULT_SENS       "sensor_data.log"

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
    {
        qDebug() << "Unable to get MODE!";
        return MODE_ERR;
    }

    return _appmode_map.value(ll, MODE_ERR);
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

QString AppConfig::owmToken()
{
    QString s = _cfg->value("owm_token", "").toString();
    return s;
}

QString AppConfig::owmURL()
{
    QString s = _cfg->value("owm_url", "").toString();
    return s;
}

QString AppConfig::owmPlace()
{
    QString s = _cfg->value("owm_place", "").toString();
    return s;
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
    QString s = _cfg->value("timeline_file", DEFAULT_TIMELINE).toString();
    QString p = QDir(_root_setting_path).filePath(s); //QString::fromLocal8Bit(s));
    return p;
}

QString AppConfig::tempDevice()
{
    QByteArray s = _cfg->value("onboard_temp", DEFAULT_TEMP).toByteArray();
    QString p = QDir(_root_intput_path).filePath(QString::fromLocal8Bit(s));
    return p;
}

QString AppConfig::sensorDevice()
{
    QByteArray s = _cfg->value("sensor_input", DEFAULT_SENS).toByteArray();
    QString p = QDir(_root_intput_path).filePath(QString::fromLocal8Bit(s));
    return p;
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

    m = _cfg->value("input_directory", DEFAULT_SETTING_DIR).toByteArray();
    _root_intput_path = QString::fromLocal8Bit(m);

    qDebug() << _root_run_path;
    qDebug() << _root_setting_path;
    qDebug() << _root_intput_path;

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


