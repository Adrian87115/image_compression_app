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

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/image_compression_app/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    QQmlContext* context(engine.rootContext());
    context->setContextProperty("GetFilePath", &m_getfilepath);
    context->setContextProperty("ButtonSignalsHandling", &m_signalhandling);

    ConvertImage result1;
    // QString result2 = result1.performConversion(true, QString::fromStdString("C:/Users/adria/Desktop/sunset.png"));
    QString result2 = result1.performConversion(false, QString::fromStdString("C:/Users/adria/Desktop/sunset_encoded.txt"));
    qDebug() << result2;
    // decoding doesnt work this way, because path to files is not correctly passed, encoding starts when play pressed, in txt mode two paths have to be uploaded
    return app.exec();
}
