#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

class Thermostat;
struct TimelineSlotsData {
    int  onOff;
    float tempSP;
};


class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)

private:
    explicit Manager(QObject *parent = nullptr);
    Thermostat * const thermostat;

public:
    static Manager *instance(QObject *parent = nullptr);

    // Temperature
    QString temperature() const;

signals:
    void temperatureChanged(int temp);
    void timelineChanged(QVector<TimelineSlotsData> *s);

public slots:
    void test(QString s);

};

#endif // MANAGER_H
