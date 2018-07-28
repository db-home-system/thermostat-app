#ifndef WEATHER_H
#define WEATHER_H

#include "appconfig.h"

#include <QObject>

class QTimer;
class QTime;
class QNetworkAccessManager;
class QNetworkReply;
class AppConfig;

struct WeatherData {
    Q_GADGET
public:
    int temp;
    int temp_max;
    int temp_min;
    int pressure;
    int humidity;
    QString icon;
    QString desc;

    Q_PROPERTY(int temp MEMBER temp)
    Q_PROPERTY(int temp_max MEMBER temp_max)
    Q_PROPERTY(int temp_min MEMBER temp_min)
    Q_PROPERTY(int pressure MEMBER pressure)
    Q_PROPERTY(int humidity MEMBER humidity)
    Q_PROPERTY(QString icon MEMBER icon)
    Q_PROPERTY(QString desc MEMBER desc)
};

Q_DECLARE_METATYPE(WeatherData)

class Weather : public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);

    int extTemp()   { return _data["now"].temp;  }
    QVariantList data();

private:
    int _temp;

    QNetworkAccessManager *const netNowMgr;
    QNetworkAccessManager *const netForecastMgr;

    bool convertWeatherIcon(QString key, QMap<QString, QVariant> buff);
    bool convertWeatherData(QString key, QString desc, QMap<QString, QVariant> buff);

    void nowRead(QNetworkReply *s);
    void nowQuery();

    void forecastRead(QNetworkReply *s);
    void forecastQuery();

    AppConfig *_cfg;
    QMap<QString, WeatherData> _data;

signals:
    void weatherNowChanged();
    void weatherForecastChanged();
};

#endif // WEATHER_H
