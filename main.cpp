#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "getfilepath.h"
#include "buttonsignalshandling.h"
#include "convertimage.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    GetFilePath m_getfilepath;
    ButtonSignalsHandling m_signalhandling;
    ConvertImage m_image_converter;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/image_compression_app/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);
    QQmlContext* context(engine.rootContext());
    context->setContextProperty("GetFilePath", &m_getfilepath);
    context->setContextProperty("ButtonSignalsHandling", &m_signalhandling);
    context->setContextProperty("ConvertImage", &m_image_converter);
    return app.exec();
}
