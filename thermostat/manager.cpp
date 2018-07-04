#include "manager.h"
#include "thermostat.h"
#include "weather.h"
#include "utils.h"

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


QString Manager::icon() const
{
    return weather->getWIcon();
}

QString Manager::tempo() const
{
    return QString::number(weather->getTemp());
}


QString Manager::displayClock() const
{
    return _diplay_clock;
}

QString Manager::intTemperature() const
{
    return QString::number(thermostat->intTemp(), 'f', 1);
}

QString Manager::extTemperature() const
{
    return QString::number(thermostat->extTemp(), 'f', 1);
}

QString Manager::thermostatStatus() const
{
   if(thermostat->status() == 1)
       return "ON";

   return "OFF";
}

void Manager::internalClockTicks()
{
    qDebug() << "Ticks elapse";

    QTime timeclock = QTime::currentTime();
    _diplay_clock = timeclock.toString("hh:mm:ss");
    int hour_clock = timeclock.hour();

    // In simulation mode overwrite current time
    if (isSimMode())
    {
        _diplay_clock = getSimTimeClock();
        QString h = _diplay_clock.split(":")[0];
        hour_clock = h.toInt();
    }

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

    int tick = 1000;
    if (isSimMode())
        tick = simTick();

    // Timer for clock display
    timer = new QTimer(this);
    timer->setInterval(tick);
    timer->start();
    connect(timer, &QTimer::timeout, this, &Manager::internalClockTicks);

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
    connect(weather, &Weather::weatherChanged, this, &Manager::weatherInfo);

}
