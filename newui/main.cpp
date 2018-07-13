//#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ImageViewer/imagemodel.h"
#include "jkenums.h"
#include "jkinterface.h"
#include "../platform/devicestruct.h"
#include <QtQml>
#include <QMenuBar>
#include <QLocale>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Faroe VOP");
    QCoreApplication::setOrganizationName("Liteon");
//    QCoreApplication::setOrganizationDomain("com.liteon");

    QTranslator trans;
    if(!trans.load(QLocale(QLocale::system().name()) ,"vop" ,"." ,":/translations")){
        trans.load(QLocale(QLocale::English),"vop" ,"." ,":/translations");
    }
    app.installTranslator(&trans);

    QTranslator trans1;
    trans1.load("qt_" + QLocale::system().name() ,":/translations");
    app.installTranslator(&trans1);

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
    jki.init(jki.getCurrentDevice());

    return app.exec();
}
