#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

class QTimer;
class Thermostat;
class Weather;
class QNetworkAccessManager;
class QNetworkReply;

struct TimelineSlotsData {
    int  onOff;
    float tempSP;
};


class Manager : public QObject
{
    Q_OBJECT

    // Thermostat property
    Q_PROPERTY(QString intTemperature READ intTemperature NOTIFY intTemperatureChanged)
    Q_PROPERTY(QString extTemperature READ extTemperature NOTIFY extTemperatureChanged)
    Q_PROPERTY(int thermostatStatus READ thermostatStatus NOTIFY thermostatStatusChanged)

    // Time property
    Q_PROPERTY(QString displayClock READ displayClock NOTIFY clockChanged)

    // Weather property
    Q_PROPERTY(QString tempo READ tempo NOTIFY weatherInfo)
    Q_PROPERTY(QString icon READ icon NOTIFY weatherInfo)

private:
    explicit Manager(QObject *parent = nullptr);

    QTimer * timer;
    Thermostat * const thermostat;
    Weather *const weather;

    int _current_h;
    QString _diplay_clock;
    void internalClockTicks();

public:
    static Manager *instance(QObject *parent = nullptr);

    // Thermostat property
    QString intTemperature() const;
    QString extTemperature() const;
    int thermostatStatus() const;

    QString displayClock() const;
    QString tempo() const;
    QString icon() const;


signals:
    void clockChanged();

    // Timeline signals
    void timelineChanged(QVector<TimelineSlotsData> &s);
    void currentHour(int h);

    // Thermostat singnals
    void thermostatStatusChanged();
    void intTemperatureChanged();
    void extTemperatureChanged();

    void weatherInfo();

public slots:
    void test();
};

#endif // MANAGER_H
