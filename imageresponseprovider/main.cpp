#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "../thumbnailviewer/imageresponseprovider.h"
#include "jkinterface.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

//    qmlRegisterType<AsyncImageProvider>("ImageResponseProviderCore" ,1 ,0 ,"AsyncImageProvider");
    qmlRegisterType<JKInterface>("com.liteon.JKInterface",1,0,"JKInterface");
    qmlRegisterType<ImageModel>("com.liteon.JKInterface",1,0,"ImageModel");

    JKInterface jki;
//    QQuickView view;
//    view.setResizeMode(QQuickView::SizeRootObjectToView);
//    QQmlContext* ctxt = view.rootContext();
//    view.engine()->addImageProvider("async" ,new AsyncImageProvider);
//    ctxt->setContextProperty("imageModel" ,&model);
//    view.setSource(QUrl("qrc:/main.qml"));
//    view.show();

    QQmlApplicationEngine engine;
    engine.addImageProvider("async" ,new AsyncImageProvider);
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("jkInterface" ,&jki);
    engine.load(QUrl(QLatin1String("qrc:/example/imageresponseprovider-example.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
