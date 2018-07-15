#include "manager.h"
#include "appconfig.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <thermostat.h>
#include <timeline.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<AppConfig>("ThermostatApp", 1, 0, "AppConfig", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return AppConfig::instance();
    });

    qmlRegisterSingletonType<Manager>("ThermostatApp", 1, 0, "Manager", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Manager::instance();
    });

    qmlRegisterType<Timeline>("ThermostatApp", 1, 0, "Timeline");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
