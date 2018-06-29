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

    double getTemp()   { return _temp; }
    QString getWIcon() { return _icon; }

private:
    QString _icon = "00d";
    double _temp = 0.0;

    QNetworkAccessManager *const netMgr;
    void getInfo(QNetworkReply *s);
    void getWeather();

signals:
    void weatherChanged();
};

#endif // WEATHER_H
