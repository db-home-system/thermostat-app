#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <thermostat.h>
#include <timeline.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Thermostat>("Thermostat", 1, 0, "Thermostat");
    qmlRegisterType<Timeline>("Timeline", 1, 0, "Timeline");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
