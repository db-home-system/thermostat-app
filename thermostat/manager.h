#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

class QTimer;
class Thermostat;
class Weather;
class QNetworkAccessManager;
class QNetworkReply;

struct TimelineSlotsData {
    int  onOff;
    float tempSP;
};


class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString intTemperature READ intTemperature NOTIFY intTemperatureChanged)
    Q_PROPERTY(QString extTemperature READ extTemperature NOTIFY extTemperatureChanged)
    Q_PROPERTY(int thermostatStatus READ thermostatStatus NOTIFY thermostatStatusChanged)
    Q_PROPERTY(QString time READ time NOTIFY timeChanged)
    Q_PROPERTY(QString tempo READ tempo NOTIFY weatherTrigger)

private:
    explicit Manager(QObject *parent = nullptr);
    QTimer * timer;
    Thermostat * const thermostat;
    Weather *weather;
    QNetworkAccessManager *mgr;

    int _current_h;

public:
    static Manager *instance(QObject *parent = nullptr);

    // Thermostat property
    QString intTemperature() const;
    QString extTemperature() const;
    int thermostatStatus() const;

    QString time() const;
    QString tempo() const;
    void random();

signals:
    void timeChanged();

    // Timeline signals
    void timelineChanged(QVector<TimelineSlotsData> *s);
    void currentHour(int h);

    // Thermostat singnals
    void thermostatStatusChanged();
    void intTemperatureChanged();
    void extTemperatureChanged();

public slots:
    void test();

    void temperatureChanged(int temp);
    void timelineChanged(QVector<int> *s);
    void weatherTrigger(QByteArray q);

};

#endif // MANAGER_H
