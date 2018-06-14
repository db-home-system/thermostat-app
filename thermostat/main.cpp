#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <thermostat.h>
#include <timeline.h>
#include "manager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Manager::intance();

    qmlRegisterType<Thermostat>("Thermostat", 1, 0, "Thermostat");
    qmlRegisterType<Timeline>("Timeline", 1, 0, "Timeline");

    qmlRegisterSingletonType<Manager>("Thermostat", 1, 0, "Manager", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
          Q_UNUSED(engine)
          Q_UNUSED(scriptEngine)

         return Manager::instance();
      });

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
