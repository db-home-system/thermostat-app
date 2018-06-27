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
    Q_PROPERTY(bool heaterStatus READ heaterStatus NOTIFY heaterStatusChanged)

private:
    explicit Manager(QObject *parent = nullptr);
    Thermostat * const thermostat;

    int _current_h;
    bool heater_status;

public:
    static Manager *instance(QObject *parent = nullptr);

    // Temperature
    QString temperature() const;
    bool heaterStatus() const;

signals:
    void temperatureChanged(int temp);
    void timelineChanged(QVector<TimelineSlotsData> *s);
    void currentHour(int h);

public slots:
    void test();
    void heaterStatusUpdate(bool status);

};

#endif // MANAGER_H
