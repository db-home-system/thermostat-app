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
    netForecastMgr(new QNetworkAccessManager(this))
{
    _cfg = AppConfig::instance();

    _data["now"] =     { NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "Now"};
    _data["next6h"] =  { NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "6h"};
    _data["next12h"] = { NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "12h"};

    _temp = NOTEMP;

    QTimer *timerNowQuery = new QTimer(this);
    connect(timerNowQuery, &QTimer::timeout, this, &Weather::nowQuery);

    QTimer *timerForecastQuery = new QTimer(this);
    connect(timerForecastQuery, &QTimer::timeout, this, &Weather::forecastQuery);

    timerNowQuery->setInterval(10000);
    timerNowQuery->start();

    timerForecastQuery->setInterval(5000);
    timerForecastQuery->start();

}

QVariantList Weather::data()
{
    QVariantList d;

    d.append(QVariant::fromValue(_data["now"]));
    d.append(QVariant::fromValue(_data["next6h"]));
    d.append(QVariant::fromValue(_data["next12h"]));

    return d;
}

bool Weather::convertWeatherIcon(QString key, QMap<QString, QVariant> buff)
{
    bool update = false;
    if (buff.empty())
    {
        qDebug() << "Empty ICONS" << buff;
        return update;
    }

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

bool Weather::convertWeatherData(QString key, QString desc, QMap<QString, QVariant> buff)
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

    qDebug() << "weather" << url;

    netNowMgr->get(QNetworkRequest(QUrl(url)));
    connect(netNowMgr, &QNetworkAccessManager::finished, this, &Weather::nowRead);
}

void Weather::nowRead(QNetworkReply *s)
{

    char const *m = s->readAll();
    QJsonDocument itemDoc = QJsonDocument::fromJson(m);
    QJsonObject obj = itemDoc.object();
    QMap<QString, QVariant> main = obj.toVariantMap();

    QDateTime clock = QDateTime::currentDateTime();
    QString desc = clock.toString("Now dd/MM");
    bool flag = convertWeatherIcon("now", main.value("weather").toJsonArray().at(0).toObject().toVariantMap());
    if (convertWeatherData("now", desc, main.value("main").toMap()) || flag)
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

    qDebug() << "forecast" << url;

    netForecastMgr->get(QNetworkRequest(QUrl(url)));
    connect(netForecastMgr, &QNetworkAccessManager::finished, this, &Weather::forecastRead);
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
    {
        qDebug() << "Forecast empty reply..";
        return;
    }

    QVector<QString> forecast({"next6h", "next12h"});
    for (auto i: forecast)
    {
        QDateTime clock = QDateTime::currentDateTime();
        int offset = 2;
        if (i == "next12h")
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
    {
        qDebug() << "forecatChanged";
        emit weatherForecastChanged();
    }
}
