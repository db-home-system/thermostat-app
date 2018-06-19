#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <QObject>
#include <QVector>

class QFileSystemWatcher;

class Thermostat : public QObject
{
    Q_OBJECT

public:
    explicit Thermostat(QObject *parent = nullptr);

public slots:
    void timelineSlots();
    void settingsChanged(QString cfg);

signals:
    void settingChanged(QVector<int> *timeline);

private:
    bool loadTimelineCfg(QString cfg, QList<QStringList> *l);


    QFileSystemWatcher * const _watcher;
    QVector<int> timeline_slots;

};

#endif // THERMOSTAT_H
