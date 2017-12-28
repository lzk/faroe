#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ImageViewer/imagemodel.h"
#include "jkenums.h"
#include "jkinterface.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ImageModel>("com.liteon.JKInterface" ,1,0,"ImageModel");
    qmlRegisterType<JKEnums>("com.liteon.JKInterface" ,1,0,"JKEnums");
    qmlRegisterType<JKInterface>("com.liteon.JKInterface",1,0,"JKInterface");

    ImageModel imageModel;
    imageModel.addImage(ImageItem("E:/tmp/pic/1.jpg" ,QSize(3200 ,2000)));
    JKInterface jki;

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("jkImageModel" ,&imageModel);
    ctxt->setContextProperty("jkInterface" ,&jki);
    engine.load(QUrl(QStringLiteral("qrc:/newui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
