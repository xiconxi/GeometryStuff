#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "PolygonPainter.h"
#include <QQmlEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<PolygonPainter>("GeometryLabs", 1, 0, "PolygonPainter");
    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
