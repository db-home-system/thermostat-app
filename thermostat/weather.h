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

private:
    QTimer *timer;
    QNetworkAccessManager *mgr;
    QNetworkReply * s;

public slots:
    void getWeather();
    void testGet(QNetworkReply* s);

signals:
    void weatherChanged(QByteArray q);
};

#endif // WEATHER_H
