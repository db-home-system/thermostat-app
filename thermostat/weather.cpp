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
    _cfg = AppConfig::instance();

    _data["now"] =     { NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "Now"};
    _data["next6h"] =  { NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "6h"};
    _data["next12h"] = { NOTEMP, NOTEMP, NOTEMP, NOPRESSURE, NOHUMIDITY, NOICON , "12h"};

    _temp = NOTEMP;

    QTimer *timer = new QTimer(this);
    timer->setInterval(5000);
    timer->start();

    connect(timer, &QTimer::timeout, this, &Weather::nowQuery);
}

QVariantList Weather::data()
{
   QVariantList d;

   d.append(QVariant::fromValue(_data["now"]));
   d.append(QVariant::fromValue(_data["next6h"]));
   d.append(QVariant::fromValue(_data["next12h"]));

   return d;
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

    netMgr->get(QNetworkRequest(QUrl(url)));
    connect(netMgr, &QNetworkAccessManager::finished, this, &Weather::nowRead);
}

void Weather::nowRead(QNetworkReply *s)
{
    bool update = false;

    char const *m = s->readAll();
    QJsonDocument itemDoc = QJsonDocument::fromJson(m);
    QJsonObject obj = itemDoc.object();
    QMap<QString, QVariant> main = obj.value("main").toObject().toVariantMap();

    QMap<QString, QVariant> weather = obj.value("weather").toArray()[0].toObject().toVariantMap();
    QString icon = weather.value("icon").toString();
    if (!icon.isEmpty() && !icon.isNull())
    {
        _data["now"].icon =  "images/icons/" + icon + ".svg";
        update = true;
    } else {
        _data["now"].icon = NOICON;
    }

    bool ok = false;
    double d = main["temp"].toDouble(&ok) * 100;
    if (ok) {
        _data["now"].temp = static_cast<int>(d);
        _temp = static_cast<int>(d);
        update = true;
    } else {
        _data["now"].temp = NOTEMP;
    }

    d = main["temp_max"].toDouble(&ok) * 100;
    if (ok) {
        _data["now"].temp_max = static_cast<int>(d);
        update = true;
    } else {
        _data["now"].temp_max = NOTEMP;
    }

    d = main["temp_min"].toDouble(&ok) * 100;
    if (ok) {
        _data["now"].temp_min = static_cast<int>(d);
        update = true;
    } else {
        _data["now"].temp_min = NOTEMP;
    }

    d = main["pressure"].toDouble(&ok) * 100;
    if (ok) {
        _data["now"].pressure = static_cast<int>(d);
        update = true;
    } else {
        _data["now"].pressure = NOPRESSURE;
    }

    d = main["humidity"].toDouble(&ok) * 100;
    if (ok) {
        _data["now"].humidity = static_cast<int>(d);
        update = true;
    } else {
        _data["now"].humidity = NOHUMIDITY;
    }

    _data["now"].desc = "Now";
    qDebug() << _temp << update;

    if (update)
        emit weatherNowChanged();
}
