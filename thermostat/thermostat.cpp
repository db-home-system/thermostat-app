#include "thermostat.h"
#include "app-config.h"

#include <QVector>
#include <QTimer>
#include <QFile>
#include <QFileSystemWatcher>

#include <QtDebug>

Thermostat::Thermostat(QObject *parent) : QObject(parent),
    _watcher(new QFileSystemWatcher(this))
{

    connect(_watcher, &QFileSystemWatcher::fileChanged,
                      this, &Thermostat::fileSettingsChanged);
    connect(_watcher, &QFileSystemWatcher::directoryChanged,
                      this, &Thermostat::dirSettingsChanged);

    dirSettingsChanged("");
    // Set correct size for timelines slots.
    timeline_slots.resize(TIMELINE_DIVISION);
}

void Thermostat::dirSettingsChanged(QString cfg)
{
    (void)cfg;

    // root path for all settings
    QString root_path = APP_SETTINGS_ROOT;
    QByteArray qroot_path = qgetenv(APP_SETTINGS_ROOT_ENV);
    if (!qroot_path.isEmpty())
        root_path = QString::fromLocal8Bit(qroot_path);

    QString settings_path = root_path + TIMELINE_CFG;
    QByteArray qsettings_path = qgetenv(TIMELINE_CFG_ENV);
    if (!qsettings_path.isEmpty())
        settings_path = QString::fromLocal8Bit(qsettings_path);

    qDebug() << root_path << settings_path;

    _watcher->addPath(root_path);
    _watcher->addPath(settings_path);
    qDebug() << _watcher->files() << _watcher->directories();

}

void Thermostat::fileSettingsChanged(QString cfg)
{
    qDebug() << "Read from: " << cfg;

    QList<QStringList> sett;
    if (loadTimelineCfg(cfg, sett))
    {
        for (int row = 0; row < sett.size(); row++)
        {
            if (sett[row].size() < 3) {
                qDebug() << "no enought elements to unpack.";
                continue;
            }

            bool *check = NULL;
            int h = sett[row][0].toInt(check);
            if ((h > 24) && !(*check)) {
                qDebug() << "Invalid hour." << h;
                continue;
            }

            int on_off = sett[row][1].toInt(check);
            if (on_off != 0 && on_off != 1 && !(*check)) {
                qDebug() << "Invalid on off value shoul be 0 or 1" << on_off;
                continue;
            }

            timeline_slots[h] = on_off;
        }
        emit settingChanged(&timeline_slots);
    }
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
