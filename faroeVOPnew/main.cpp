#include "mainwindow.h"
#include <QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "../qml/jkenums.h"
#include "../qml/ImageViewer/imagemodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<ImageModel>("com.liteon.JKInterface" ,1,0,"ImageModel");
    qmlRegisterType<JKEnums>("com.liteon.JKInterface" ,1,0,"JKEnums");
    ImageModel imageModel;
    imageModel.addImage(ImageItem("E:/tmp/pic/1.jpg" ,QSize(3200 ,2000)));

    QQuickView* view = new QQuickView;
    QQmlContext* ctxt = view->engine()->rootContext();
    ctxt->setContextProperty("jkImageModel" ,&imageModel);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl(QLatin1String("qrc:/newui/MainView.qml")));
    view->connect(view->engine() ,&QQmlEngine::quit ,&a ,&QCoreApplication::quit);

    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setCentralWidget(QWidget::createWindowContainer(view));
    w.resize(850 ,648);

    QQuickItem* rootObject = view->rootObject();
    QObject::connect(rootObject ,SIGNAL(closed()) ,&w ,SLOT(close()));
    QObject::connect(rootObject ,SIGNAL(minimized()) ,&w ,SLOT(showMinimized()));
    QObject::connect(rootObject ,SIGNAL(move(qreal,qreal)) ,&w ,SLOT(mmove(qreal,qreal)));
    w.show();

    return a.exec();
}
