#ifndef MANAGER_H
#define MANAGER_H

#include "utils.h"
#include "weather.h"
#include "appconfig.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QVariantList>

class QTimer;
class Thermostat;
class Weather;
class QNetworkAccessManager;
class QNetworkReply;
struct WeatherData;

struct TimelineSlotsData {
    int  onOff;
    int tempSP;
};


class Manager : public QObject
{
    Q_OBJECT

    // Thermostat property
    Q_PROPERTY(int intTemperature READ intTemperature NOTIFY intTemperatureChanged)
    Q_PROPERTY(int extTemperature READ extTemperature NOTIFY extTemperatureChanged)
    Q_PROPERTY(QString thermostatStatus READ thermostatStatus NOTIFY thermostatStatusChanged)

    // Time property
    Q_PROPERTY(QString displayClock READ displayClock NOTIFY clockChanged)

    // Weather property
    Q_PROPERTY(QVariantList weatherData READ weatherData NOTIFY weatherInfo)

private:
    explicit Manager(QObject *parent = nullptr);

    QTimer *_clock_tick;
    Thermostat * const thermostat;
    Weather *const weather;

    int _current_h;
    int _thermo_status;

    QString _diplay_clock;
    void internalClockTicks();
    void internalThermoStatus();

    AppConfig *_cfg;

public:
    static Manager *instance(QObject *parent = nullptr);

    // Thermostat property
    int intTemperature() const;
    int extTemperature() const;
    QString thermostatStatus() const;

    QString displayClock() const;
    QVariantList weatherData() const;

signals:
    void clockChanged();

    // Timeline signals
    void timelineChanged(QVector<TimelineSlotsData> &s);
    void currentHour(int h);
    void currentStatus(int status);

    // Thermostat singnals
    void thermostatStatusChanged();
    void intTemperatureChanged();
    void extTemperatureChanged();

    void weatherInfo();

public slots:
    void test();
};

#endif // MANAGER_H
