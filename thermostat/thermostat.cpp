#include "thermostat.h"

Thermostat::Thermostat(QObject *parent) : QObject(parent)
{
    temp = "20°C";

}

QString Thermostat::temperature() const
{
    return temp;

}

void Thermostat::setTemperature(const QString &t)
{
    if (temp != t) {
        temp = t;
        emit temperatureChanged();
    }
}
