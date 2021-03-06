#include "appconfig.h"
#include "manager.h"
#include "thermostat.h"
#include "weather.h"
#include "utils.h"
#include "appconfig.h"

#include <QFile>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QTime>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QtDebug>

Manager *Manager::instance(QObject *parent)
{
    static Manager *manager_instance = new Manager(parent);

    return manager_instance;
}

QVariantList Manager::weatherData() const
{
    return weather->data();
}

QString Manager::displayClock() const
{
    return _diplay_clock;
}

int Manager::intTemperature() const
{
    return thermostat->intTemp();
}

int Manager::extTemperature() const
{
    int temp = NOTEMP;
    if (thermostat->extTemp() != NOTEMP)
        return thermostat->extTemp();

    if (weather->extTemp() != NOTEMP)
        return weather->extTemp();

    return temp;
}

QString Manager::thermostatStatus() const
{
   if(thermostat->status() == 1)
       return "ON";

   return "OFF";
}

void Manager::internalClockTicks()
{
    QTime timeclock = QTime::currentTime();
    _diplay_clock = timeclock.toString("hh:mm:ss");

    int tick = DEFAULT_CLOCK_TICK;
    if (_cfg->appMode() == AppConfig::TEST)
    {
        _diplay_clock = _cfg->getSimTimeClock();
        tick = _cfg->simTick();
    }

    int hour_clock = _diplay_clock.split(":")[0].toInt();
    _clock_tick->setInterval(tick);

    if (hour_clock != _current_h)
    {
      _current_h = hour_clock;
      if (_current_h > 23)
          _current_h = 0;

      thermostat->updateHour(_current_h);
      emit currentHour(_current_h);
    }

    emit clockChanged();
}

void Manager::internalThermoStatus()
{
    emit currentStatus(thermostat->status());
}

void Manager::test()
{
}

Manager::Manager(QObject *parent) : QObject(parent),
  thermostat(new Thermostat(this)), weather(new Weather(this))
{
    // Timeline mark updater
    _current_h = 0;
    _thermo_status = 0;

    // Timer for clock display
    _clock_tick = new QTimer(this);
    _clock_tick->setInterval(DEFAULT_CLOCK_TICK);
    _clock_tick->start();
    connect(_clock_tick, &QTimer::timeout, this, &Manager::internalClockTicks);

    // Signal to manage timeline and temperature
    connect(thermostat, &Thermostat::dataChanged,
                    this, &Manager::timelineChanged);

    connect(thermostat, &Thermostat::statusChanged,
                    this, &Manager::thermostatStatusChanged);

    connect(thermostat, &Thermostat::statusChanged,
                    this, &Manager::intTemperatureChanged);

    connect(thermostat, &Thermostat::statusChanged,
                    this, &Manager::extTemperatureChanged);

    connect(thermostat, &Thermostat::statusChanged,
                    this, &Manager::internalThermoStatus);

    // Weather info signals
    connect(weather, &Weather::weatherNowChanged, this, &Manager::weatherInfo);
    connect(weather, &Weather::weatherForecastChanged, this, &Manager::weatherInfo);

    _cfg = AppConfig::instance();
}
