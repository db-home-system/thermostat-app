#include "weather.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Weather::Weather(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    timer->setInterval(5000);
    timer->start();

    connect(timer, &QTimer::timeout, this, &Weather::getWeather);

}

void Weather::getWeather()
{
    mgr = new QNetworkAccessManager(this);
    mgr->get(QNetworkRequest(QUrl("http://api.openweathermap.org/data/2.5/forecast/daily?id=4062577&units=metric&appid=8f3edecd56bac0612c3c92a1b177d306")));
    connect(mgr, &QNetworkAccessManager::finished, this, &Weather::getInfo);
}

double Weather::getTemp()
{
    return temp;
}

void Weather::setTemp(float value)
{
    temp = value;
}

void Weather::getInfo(QNetworkReply *s)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(s->readAll());
    QJsonObject obj = itemDoc.object();
    QJsonValue x = obj.value("list").toArray()[0].toObject()["temp"];
    x = x["day"];
    qDebug() << x.toDouble();

    setTemp(x.toDouble());
    emit weatherChanged();
}
