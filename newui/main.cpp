//#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ImageViewer/imagemodel.h"
#include "jkenums.h"
#include "jkinterface.h"
#include "../platform/devicestruct.h"

#include <QMenuBar>
int main(int argc, char *argv[])
{
//    QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Faroe VOP");
    QCoreApplication::setOrganizationName("Liteon");
//    QCoreApplication::setOrganizationDomain("com.liteon");

    qmlRegisterType<ImageModel>("com.liteon.JKInterface" ,1,0,"ImageModel");
    qmlRegisterType<JKEnums>("com.liteon.JKInterface" ,1,0,"JKEnums");
    qmlRegisterType<JKInterface>("com.liteon.JKInterface",1,0,"JKInterface");
    qmlRegisterType<DeviceStruct>("com.liteon.JKInterface",1,0,"DeviceStruct");

    ImageModel imageModel;
    JKInterface jki;
    jki.installImageModel(&imageModel);

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("jkImageModel" ,&imageModel);
    ctxt->setContextProperty("jkInterface" ,&jki);
    engine.load(QUrl(QStringLiteral("qrc:/newui/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

//    QObject* rootObject = engine.rootObjects()[0];
//    QObject* scanData = rootObject->findChild<QObject*>("scanData");
//    if(scanData)
//        jki.setScanDataHandle(scanData);
    jki.init();


    //1. 获取菜单栏
    QMenuBar* menuBar = new QMenuBar(0);
    //2. 给菜单栏添加菜单
    QMenu *wnd = menuBar->addMenu("Window");
    //3. 给菜单添加菜单项
    wnd->addSeparator();
    menuBar->setVisible(false);
    return app.exec();
}
