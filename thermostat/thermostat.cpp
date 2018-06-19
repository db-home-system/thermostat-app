#include "thermostat.h"

#include <QVector>
#include <QTimer>
#include <QFile>
#include <QFileSystemWatcher>

#include <QtDebug>

Thermostat::Thermostat(QObject *parent) : QObject(parent),
    _watcher(new QFileSystemWatcher(this))
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timelineSlots()));
    timer->start(1000);

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

    QList<QStringList> wordList;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        QString lline = QString::fromLocal8Bit(line);
        wordList.append(lline.split(';'));
    }
    qDebug() << wordList;
    file.close();

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
