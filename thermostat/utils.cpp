#include "utils.h"
#include "app-config.h"

#include <QObject>
#include <QFile>
#include <QDebug>

bool isSimMode()
{
    QByteArray mode = qgetenv("APP_SIM_MODE");
    if (!mode.isEmpty())
        if (QString::fromLocal8Bit(mode) == "1")
            return true;

    return false;
}

QString settingsRootPath()
{
    // root path for all settings
    QString p = "/etc/thermapp/";
    QByteArray qroot_path = qgetenv("THERMAPP_SETTINGS_ROOT");
    if (!qroot_path.isEmpty())
        p = QString::fromLocal8Bit(qroot_path);

    return p;
}

QString settingsPath()
{
    QString p = settingsRootPath() + "timeline.cfg";
    QByteArray qsettings_path = qgetenv("THERAPP_TIMELINE_CFG");
    if (!qsettings_path.isEmpty())
        p = QString::fromLocal8Bit(qsettings_path);

    return p;
}

QString simRootPath()
{
    // root path for all settings
    QString p = "/tmp/sim/";
    QByteArray qroot_path = qgetenv("APP_SIM_ROOT_PATH");
    if (!qroot_path.isEmpty())
        p = QString::fromLocal8Bit(qroot_path);

    return p;
}

QString simPath()
{
    QString p = simRootPath() + "time.cfg";
    QByteArray qsettings_path = qgetenv("APP_SIM_TIME");
    if (!qsettings_path.isEmpty())
        p = QString::fromLocal8Bit(qsettings_path);

    return p;
}

int simTick()
{
    int p = 100;
    QByteArray tick = qgetenv("APP_SIM_TICK");
    if (!tick.isEmpty())
    {
        QString s = QString::fromLocal8Bit(tick);
        p = s.toInt();
    }

    return p;
}

QString getTimeClock()
{
    QString l = "00:00:00";

    QFile file(simPath());
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return l;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        l = line.trimmed();
    }
    file.close();

    return l;
}
