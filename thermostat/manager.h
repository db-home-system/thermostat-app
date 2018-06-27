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
    Q_PROPERTY(QString intTemperature READ intTemperature NOTIFY intTemperatureChanged)
    Q_PROPERTY(QString extTemperature READ extTemperature NOTIFY extTemperatureChanged)
    Q_PROPERTY(int thermostatStatus READ thermostatStatus NOTIFY thermostatStatusChanged)

private:
    explicit Manager(QObject *parent = nullptr);
    Thermostat * const thermostat;

    int _current_h;

public:
    static Manager *instance(QObject *parent = nullptr);

    // Thermostat property
    QString intTemperature() const;
    QString extTemperature() const;
    int thermostatStatus() const;

signals:
    // Timeline signals
    void timelineChanged(QVector<TimelineSlotsData> *s);
    void currentHour(int h);

    // Thermostat singnals
    void thermostatStatusChanged();
    void intTemperatureChanged();
    void extTemperatureChanged();

public slots:
    void test();

};

#endif // MANAGER_H
