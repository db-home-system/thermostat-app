#include "thermostat.h"

#include <QVector>
#include <QTimer>
#include <QFile>
#include <QFileSystemWatcher>

#include <QtDebug>

Thermostat::Thermostat(QObject *parent) : QObject(parent),
    _watcher(new QFileSystemWatcher(this))
{

    QString watch_path = "/etc/therapp/timeline.cfg";
    QByteArray settings_path = qgetenv("THERAPP_TIMELINE");
    if (!settings_path.isEmpty())
        watch_path = QString::fromLocal8Bit(settings_path);

    // add root path to watch all files.
    qDebug() << watch_path;
    _watcher->addPath(watch_path);
    qDebug() << _watcher->files();

    connect(_watcher, &QFileSystemWatcher::fileChanged,
                      this, &Thermostat::settingsChanged);
}

void Thermostat::settingsChanged(QString cfg)
{
    qDebug() << "file..";
    qDebug() << cfg;

    timeline_slots.resize(24);
    QList<QStringList> sett;
    if (loadTimelineCfg(cfg, &sett))
    {
        for (int row = 0; row < sett.size(); row++)
        {
            if (sett[row].size() < 3) {
                qDebug() << "no enought elements to unpack.";
                continue;
            }

            int h = sett[row][0].toInt();
            if (h > 24) {
                qDebug() << "Invalid hour." << h;
                continue;
            }

            int on_off = sett[row][1].toInt();
            if (on_off != 0 && on_off != 1) {
                qDebug() << "Invalid on off value shoul be 0 or 1" << on_off;
                continue;
            }

            timeline_slots[h] = on_off;
        }
        emit settingChanged(&timeline_slots);
    }
}

bool Thermostat::loadTimelineCfg(QString cfg, QList<QStringList> *l)
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
        l->append(lline.split(';'));
        qDebug() << lline;
    }
    file.close();

    return true;
}

void Thermostat::timelineSlots()
{
//    QVector<int> timeline_slots(24, 0);
//    int flag = 0;
//    for (int j = 0; j < timeline_slots.size(); ++j) {
//        timeline_slots[j] = flag;
//        flag = !flag;
//    }
//    emit settingChanged(&timeline_slots);
}
