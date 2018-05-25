#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <QObject>

class Thermostat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)

public:
    explicit Thermostat(QObject *parent = nullptr);
    QString temperature() const;
    void setTemperature(const QString &t);

signals:
    void temperatureChanged();

private:
    QString temp;
};

#endif // THERMOSTAT_H
