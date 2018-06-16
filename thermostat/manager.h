#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

class Thermostat;

class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)

private:
    explicit Manager(QObject *parent = nullptr);

public:
    static Manager *instance(QObject *parent = nullptr);

    // Temperature
    QString temperature() const;

signals:
    void temperatureChanged(int temp);

public slots:
};

#endif // MANAGER_H
