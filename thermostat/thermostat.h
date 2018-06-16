#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <QObject>

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
    bool loadTimelineCfg(QString cfg);

};

#endif // THERMOSTAT_H
