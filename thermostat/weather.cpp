#include "weather.h"
#include <QTimer>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Weather::Weather(QObject *parent) : QObject(parent),
    netNowMgr(new QNetworkAccessManager(this)),
    netForecastMgr(new QNetworkAccessManager(this)),
    _timerNowQuery(new QTimer(this)),
    _timerForecastQuery(new QTimer(this))
{
    _cfg = AppConfig::instance();

    _data.append(WeatherData{ NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "Now"});
    _data.append(WeatherData{ NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "Next 6h"});
    _data.append(WeatherData{ NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "Next 12h"});

    connect(_timerNowQuery, &QTimer::timeout, this, &Weather::nowQuery);
    connect(_timerForecastQuery, &QTimer::timeout, this, &Weather::forecastQuery);

    _timerNowQuery->setInterval(100);
    _timerNowQuery->start();

    _timerForecastQuery->setInterval(1000);
    _timerForecastQuery->start();
}

QVariantList Weather::data()
{
    QVariantList d;

    d.append(QVariant::fromValue(_data[WEATHER_NOW]));
    d.append(QVariant::fromValue(_data[WEATHER_NEXT1]));
    d.append(QVariant::fromValue(_data[WEATHER_NEXT2]));

    return d;
}

bool Weather::convertWeatherIcon(WeatherDataEnum key, QMap<QString, QVariant> buff)
{
    bool update = false;
    if (buff.empty())
        return update;

    QString icon = buff.value("icon").toString();
    if (!icon.isEmpty() && !icon.isNull())
    {
        _data[key].icon =  "images/icons/" + icon + ".svg";
        update = true;
    } else {
        _data[key].icon = NOICON;
    }

    return update;
}

bool Weather::convertWeatherData(WeatherDataEnum key, QString desc, QMap<QString, QVariant> buff)
{
    bool update = false;
    bool ok = false;

    if (buff.empty())
    {
        qDebug() << "Empty data";
        return update;
    }

    double d = buff["temp"].toDouble(&ok);
    if (ok) {
        _data[key].temp = static_cast<int>(d * 100);
        update = true;
    } else {
        _data[key].temp = NOTEMP;
    }

    d = buff["temp_max"].toDouble(&ok);
    if (ok) {
        _data[key].temp_max = static_cast<int>(d * 100);
        update = true;
    } else {
        _data[key].temp_max = NOTEMP;
    }

    d = buff["temp_min"].toDouble(&ok);
    if (ok) {
        _data[key].temp_min = static_cast<int>(d * 100);
        update = true;
    } else {
        _data[key].temp_min = NOTEMP;
    }

    d = buff["pressure"].toDouble(&ok);
    if (ok) {
        _data[key].pressure = static_cast<int>(d * 100);
        update = true;
    } else {
        _data[key].pressure = NOPRESSURE;
    }

    d = buff["humidity"].toDouble(&ok);
    if (ok) {
        _data[key].humidity = static_cast<int>(d * 100);
        update = true;
    } else {
        _data[key].humidity = NOHUMIDITY;
    }

    _data[key].desc = desc;

    return update;
}

void Weather::nowQuery()
{
    if (_cfg->owmURL().isEmpty() ||
            _cfg->owmPlace().isEmpty() ||
            _cfg->owmToken().isEmpty())
    {
        qDebug() << "Invalid url for open weather.";
        return;
    }

    QString url = _cfg->owmURL();
    url += "weather";
    url += "?q=" + _cfg->owmPlace();
    url += "&unit=metric";
    url += "&appid=" + _cfg->owmToken();

//    qDebug() << "weather" << url;

    netNowMgr->get(QNetworkRequest(QUrl(url)));
    connect(netNowMgr, &QNetworkAccessManager::finished, this, &Weather::nowRead);

    _timerNowQuery->setInterval(DEFAULT_WEATHER_POLL_TIME);

}

void Weather::nowRead(QNetworkReply *s)
{

    char const *m = s->readAll();
    QJsonDocument itemDoc = QJsonDocument::fromJson(m);
    QJsonObject obj = itemDoc.object();
    QMap<QString, QVariant> main = obj.toVariantMap();

    QDateTime clock = QDateTime::currentDateTime();
    QString desc = clock.toString("Now dd/MM");

    bool flag = convertWeatherIcon(WEATHER_NOW, main.value("weather").toJsonArray().at(0).toObject().toVariantMap());
    if (convertWeatherData(WEATHER_NOW, desc, main.value("main").toMap()) || flag)
        emit weatherNowChanged();
}

void Weather::forecastQuery()
{
    if (_cfg->owmURL().isEmpty() ||
            _cfg->owmPlace().isEmpty() ||
            _cfg->owmToken().isEmpty())
    {
        qDebug() << "Invalid url for open weather.";
        return;
    }

    QString url = _cfg->owmURL();
    url += "forecast";
    url += "?q=" + _cfg->owmPlace();
    url += "&cnt=10";
    url += "&unit=metric";
    url += "&appid=" + _cfg->owmToken();

//    qDebug() << "forecast" << url;

    netForecastMgr->get(QNetworkRequest(QUrl(url)));
    connect(netForecastMgr, &QNetworkAccessManager::finished, this, &Weather::forecastRead);

    _timerForecastQuery->setInterval(DEFAULT_FORECAST_POLL_TIME);
}

void Weather::forecastRead(QNetworkReply *s)
{
    bool update = false;

    char const *m = s->readAll();
    QJsonDocument itemDoc = QJsonDocument::fromJson(m);
    QJsonObject obj = itemDoc.object();

    int index = 0;
    QJsonArray l = obj.value("list").toArray();
    if (l.isEmpty())
        return;

    QVector<WeatherDataEnum> forecast({WEATHER_NEXT1, WEATHER_NEXT2});
    for (auto i: forecast)
    {
        QDateTime clock = QDateTime::currentDateTime();
        int offset = 2;
        if (i == WEATHER_NEXT2)
            offset = 8;
        QString desc = clock.addSecs(offset * 3 * 3600).toString("hh:00 dd/MM");

        if ((index + offset) >= l.size())
        {
            qDebug() << "Invalid Offset!" << offset;
            continue;
        }

        QMap<QString, QVariant> data = l[index + offset].toObject().toVariantMap();
        bool flag = convertWeatherIcon(i, data.value("weather").toJsonArray().at(0).toObject().toVariantMap());
        if (convertWeatherData(i, desc, data["main"].toMap()) || flag)
            update = true;
    }

    if (update)
        emit weatherForecastChanged();
}
