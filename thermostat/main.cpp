#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <thermostat.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Thermostat>("com.thermostat", 1, 0, "Thermostat");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
