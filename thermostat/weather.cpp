#include "weather.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

Weather::Weather(QObject *parent) : QObject(parent)
{
    mgr = new QNetworkAccessManager();
    timer = new QTimer();
    timer->setInterval(5000);
    timer->start();

    connect(timer, &QTimer::timeout, this, &Weather::getWeather);

}

void Weather::getWeather()
{
    mgr->get(QNetworkRequest(QUrl("http://api.openweathermap.org/data/2.5/forecast/daily?id=4062577&units=metric&appid=8f3edecd56bac0612c3c92a1b177d306")));
    connect(mgr, &QNetworkAccessManager::finished, this, &Weather::testGet);
}

void Weather::testGet(QNetworkReply* s)
{
    qDebug() << s->readAll();
    emit weatherChanged(s->readAll());

}
