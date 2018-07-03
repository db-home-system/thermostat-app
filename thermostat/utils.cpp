#include "utils.h"
#include "app-config.h"

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QStringList>

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

    qDebug() << p;
    return p;
}

QString settingsPath()
{
    QString p = settingsRootPath() + "timeline.cfg";
    QByteArray qsettings_path = qgetenv("THERAPP_TIMELINE_CFG");
    if (!qsettings_path.isEmpty())
        p = QString::fromLocal8Bit(qsettings_path);

    qDebug() << p;
    return p;
}

QString inputsRootPath()
{
    QString p = "/var/lib/thermapp/input/";
    QByteArray qroot_path = qgetenv("THERMAPP_INPUT_ROOT");
    if (!qroot_path.isEmpty())
        p = QString::fromLocal8Bit(qroot_path);

    return p;
}

QString outputRootPath()
{
    QString p = "/var/lib/thermapp/input/";
    QByteArray qroot_path = qgetenv("THERMAPP_OUTPUT_ROOT");
    if (!qroot_path.isEmpty())
        p = QString::fromLocal8Bit(qroot_path);

    return p;
}

bool writeLineToFile(QString f, QString data)
{
    QFile file(f);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return false;
    }

    data += "\n";
    file.write(data.toUtf8());
    file.close();

    return true;
}


QString readLineFromFile(QString f)
{
    QString l = "";
    QFile file(f);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return l;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        l = QString::fromLocal8Bit(line);
    }
    file.close();

    return l;
}

bool readCSVFile(QString f, QList<QStringList> &l)
{
    QFile file(f);
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

QString getSimTimeClock()
{
    QString ll = readLineFromFile(simPath());
    if (ll == "")
        return "00:00:00";

    return ll;
}
