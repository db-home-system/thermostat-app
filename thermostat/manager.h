#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

class Thermostat;

class Manager : public QObject
{
    Q_OBJECT
private:
    explicit Manager(QObject *parent = nullptr);

    Thermostat *thermostat;

public:
    static Manager *instance(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MANAGER_H
