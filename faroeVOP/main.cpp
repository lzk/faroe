#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include "../thumbnailviewer/imageresponseprovider.h"
#include "jkinterface.h"
#include "../platform/devicestruct.h"
#include <QQuickView>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);


    qmlRegisterType<JKInterface>("com.liteon.JKInterface",1,0,"JKInterface");
    qmlRegisterType<ImageModel>("com.liteon.JKInterface",1,0,"ImageModel");
    qmlRegisterType<DeviceStruct>("com.liteon.JKInterface",1,0,"DeviceStruct");
//    qmlRegisterType<StringModel>("com.liteon.JKInterface",1,0,"StringModel");

    JKInterface jki;

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("jkInterface" ,&jki);
    engine.load(QUrl(QLatin1String("qrc:/faroeVOP/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

//    QQuickView* view = new QQuickView;
//    view->engine()->rootContext()->setContextProperty("jkInterface" ,&jki);
//    view->setResizeMode(QQuickView::SizeRootObjectToView);
//    view->setSource(QUrl(QLatin1String("qrc:/faroeVOP/MainWindow.qml")));
//    view->connect(view->engine() ,&QQmlEngine::quit ,&app ,&QCoreApplication::quit);
//    QMainWindow window;
//    window.resize(1024 ,768);
//    window.setCentralWidget(QWidget::createWindowContainer(view));
//    window.show();

    return app.exec();
}
