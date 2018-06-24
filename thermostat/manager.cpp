#include "manager.h"
#include "thermostat.h"

#include <QtDebug>
#include <QFile>
#include <QTimer>
#include <QFileSystemWatcher>

Manager *Manager::instance(QObject *parent)
{
    static Manager *manager_instance = new Manager(parent);

    return manager_instance;
}

QString Manager::temperature() const
{
    return "25ºC";
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
  thermostat(new Thermostat(this))
{
    connect(thermostat, &Thermostat::dataChanged,
                    this, &Manager::timelineChanged);

    QTimer *tt = new QTimer(this);
    connect(tt, &QTimer::timeout, this, &Manager::test);
    tt->start(300);

    _current_h = 0;
}
