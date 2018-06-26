#ifndef WEATHER_H
#define WEATHER_H
#include <QObject>
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
class Weather : public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);
    double getTemp();
    void setTemp(float value);

private:
    double temp = 5.0;
    QTimer *timer;
    QNetworkAccessManager *mgr;

public slots:
    void getWeather();
    void getInfo(QNetworkReply *s);

signals:
    void weatherChanged();
};

#endif // WEATHER_H
