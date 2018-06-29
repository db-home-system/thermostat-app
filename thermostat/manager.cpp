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


QString Manager::time() const
{
    QTime timeclock = QTime::currentTime();
    QString text = timeclock.toString("hh:mm:ss");
    return text;
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

void Manager::test()
{
    qDebug() << "test slot";
    _current_h++;
    if (_current_h > 23)
        _current_h = 0;

    emit currentHour(_current_h);
}

Manager::Manager(QObject *parent) : QObject(parent),
  thermostat(new Thermostat(this)), weather(new Weather(this))
{
    // Timer for clock display
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout, this, &Manager::timeChanged);

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

    _current_h = 0;

    QTimer *tt = new QTimer(this);
    connect(tt, &QTimer::timeout, this, &Manager::test);
    tt->start(300);
}
