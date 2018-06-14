#include "manager.h"

#include "thermostat.h"

Manager *Manager::instance(QObject *parent)
{
    static Manager *manager_instance = new Manager(parent);

    return manager_instance;
}


Manager::Manager(QObject *parent) : QObject(parent)
{

}
