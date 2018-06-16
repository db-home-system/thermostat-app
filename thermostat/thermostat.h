#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <QObject>

class Thermostat : public QObject
{
    Q_OBJECT

public:
    explicit Thermostat(QObject *parent = nullptr);
signals:


private:
};

#endif // THERMOSTAT_H
