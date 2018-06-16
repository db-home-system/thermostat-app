#include "manager.h"

#include "thermostat.h"

Manager *Manager::instance(QObject *parent)
{
    static Manager *manager_instance = new Manager(parent);

    return manager_instance;
}

QString Manager::temperature() const
{
    return "25ºC";
}


Manager::Manager(QObject *parent) : QObject(parent)
{

}
