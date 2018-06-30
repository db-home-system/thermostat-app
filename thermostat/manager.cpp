#include "manager.h"
#include "thermostat.h"
#include "weather.h"
#include <QtDebug>
#include <QFile>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QTime>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

int Manager::thermostatStatus() const
{
   return thermostat->status();
}

void Manager::internalClockTicks()
{
    qDebug() << "Ticks elapse";

    QTime timeclock = QTime::currentTime();
    _diplay_clock = timeclock.toString("hh:mm:ss");

    if (timeclock.hour() != _current_h)
    {
      _current_h = timeclock.hour();
      if (_current_h > 23)
          _current_h = 0;

      emit currentHour(_current_h);
    }

    emit clockChanged();
}

void Manager::test()
{
}

Manager::Manager(QObject *parent) : QObject(parent),
  thermostat(new Thermostat(this)), weather(new Weather(this))
{
    // Timeline mark updater
    _current_h = 0;

    // Timer for clock display
    timer = new QTimer(this);
    timer->setInterval(1000);
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

    // Weather info signals
    connect(weather, &Weather::weatherChanged, this, &Manager::weatherInfo);

}
