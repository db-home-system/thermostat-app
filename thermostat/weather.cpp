#include "weather.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Weather::Weather(QObject *parent) : QObject(parent),
  netMgr(new QNetworkAccessManager(this))
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(5000);
    timer->start();

    connect(timer, &QTimer::timeout, this, &Weather::getWeather);

}

void Weather::getWeather()
{
    netMgr->get(QNetworkRequest(QUrl("http://api.openweathermap.org/data/2.5/forecast/daily?id=4062577&units=metric&appid=8f3edecd56bac0612c3c92a1b177d306")));
    connect(netMgr, &QNetworkAccessManager::finished, this, &Weather::getInfo);
}

void Weather::getInfo(QNetworkReply *s)
{
    bool flag = false;
    QJsonDocument itemDoc = QJsonDocument::fromJson(s->readAll());
    QJsonObject obj = itemDoc.object();

    QJsonValue lfTemp = obj.value("list").toArray()[0].toObject()["temp"];
    double temp = lfTemp["day"].toDouble();

    if (_temp != temp)
    {
        _temp = temp;
        flag = true;
    }

    QJsonValue lfIcon = obj.value("list").toArray()[0].toObject().value("weather").toArray()[0];
    QString icon = lfIcon["icon"].toString();
    if (icon != _icon)
    {
        _icon = icon;
        flag = true;
    }

    if (flag)
        emit weatherChanged();
}
