#include "mainwindow.h"
#include <QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "../newui/jkenums.h"
#include "../newui/ImageViewer/imagemodel.h"
#include "../newui/jkinterface.h"
#include "../platform/devicestruct.h"
#include <QHBoxLayout>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Faroe VOP");
    QCoreApplication::setOrganizationName("Liteon");
    QCoreApplication::setOrganizationDomain("com.liteon");

    QTranslator trans;
    if(!trans.load(QLocale(QLocale::system().name()) ,"vop" ,"." ,":/translations"))
        trans.load(QLocale(QLocale::English) ,"vop" ,"." ,":/translations");

    a.installTranslator(&trans);

    qmlRegisterType<ImageModel>("com.liteon.JKInterface" ,1,0,"ImageModel");
    qmlRegisterType<JKEnums>("com.liteon.JKInterface" ,1,0,"JKEnums");
    qmlRegisterType<JKInterface>("com.liteon.JKInterface",1,0,"JKInterface");
    qmlRegisterType<DeviceStruct>("com.liteon.JKInterface",1,0,"DeviceStruct");

    ImageModel imageModel;
    JKInterface jki;
    jki.installImageModel(&imageModel);

    QQuickView* view = new QQuickView;
    QQmlContext* ctxt = view->engine()->rootContext();
    ctxt->setContextProperty("jkImageModel" ,&imageModel);
    ctxt->setContextProperty("jkInterface" ,&jki);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl(QLatin1String("qrc:/newui/MainView.qml")));
    view->connect(view->engine() ,&QQmlEngine::quit ,&a ,&QCoreApplication::quit);

    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);
//    w.setCentralWidget(QWidget::createWindowContainer(view));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(QWidget::createWindowContainer(view));
    layout->setContentsMargins(0, 0, 0, 0);
    w.setLayout(layout);
    w.setFixedSize(850 ,638);

    QQuickItem* rootObject = view->rootObject();
    QObject::connect(rootObject ,SIGNAL(closed()) ,&w ,SLOT(close()));
    QObject::connect(rootObject ,SIGNAL(minimized()) ,&w ,SLOT(showMinimized()));
    QObject::connect(rootObject ,SIGNAL(move(qreal,qreal)) ,&w ,SLOT(mmove(qreal,qreal)));
    w.show();
    rootObject->setProperty("originCenterX", w.x() + w.width()/2);
    rootObject->setProperty("originCenterY", w.y() + w.height()/2);

    QObject* scanData = rootObject->findChild<QObject*>("scanData");
    if(scanData)
        jki.setScanDataHandle(scanData);

    return a.exec();
}
