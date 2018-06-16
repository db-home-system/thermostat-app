#include "thermostat.h"

#include <QVector>
#include <QTimer>
#include <QFile>
#include <QFileSystemWatcher>

#include <QtDebug>

Thermostat::Thermostat(QObject *parent) : QObject(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timelineSlots()));
    timer->start(1000);

    // Watch configuration file
    QFileSystemWatcher watcher;

    QString watch_path = "/etc/therapp/timeline.cfg";
    QByteArray settings_path = qgetenv("THERAPP_TIMELINE");
    if (!settings_path.isEmpty())
        watch_path = QString::fromLocal8Bit(settings_path);

    watcher.addPath(watch_path);
    qDebug() << watcher.files();

    connect(&watcher, SIGNAL(fileChanged(QString)),
                      this, SLOT(settingsChanged(QString)));

    loadTimelineCfg(watch_path);
}

void Thermostat::settingsChanged(QString cfg)
{
    qDebug() << "file..";
    qDebug() << cfg;

    loadTimelineCfg(cfg);
}

bool Thermostat::loadTimelineCfg(QString cfg)
{
    QFile file(cfg);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return false;
    }

    QStringList wordList;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        wordList.append(line.split(';').first());
    }
    qDebug() << wordList;

    return true;
}

void Thermostat::timelineSlots()
{
    QVector<int> timeline_slots(24, 0);
    int flag = 0;
    for (int j = 0; j < timeline_slots.size(); ++j) {
        timeline_slots[j] = flag;
        flag = !flag;
    }
    emit settingChanged(&timeline_slots);
}
