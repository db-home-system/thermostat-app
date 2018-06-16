#include "manager.h"
#include "thermostat.h"

#include <QtDebug>
#include <QFile>
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

void Manager::test(QString s)
{
    qDebug() << "test slot" << s;
}

Manager::Manager(QObject *parent) : QObject(parent)
{
    Thermostat *thermostat = new Thermostat();
    connect(thermostat, SIGNAL(settingChanged(QVector<int>*)),
                    this, SIGNAL(timelineChanged(QVector<int>*)));
}
