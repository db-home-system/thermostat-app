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
    QString getWIcon();
    void setWIcon(QString value);

private:
    QString wIcon = "00d";
    double temp = 0.0;
    QTimer *timer;
    QNetworkAccessManager *mgr;

public slots:
    void getWeather();
    void getInfo(QNetworkReply *s);

signals:
    void weatherChanged();
};

#endif // WEATHER_H
