#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include "../platform/devicestruct.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qmlRegisterType<JKInterface>("com.liteon.JKInterface",1,0,"JKInterface");
    qmlRegisterType<ImageModel>("com.liteon.JKInterface",1,0,"ImageModel");
    qmlRegisterType<DeviceStruct>("com.liteon.JKInterface",1,0,"DeviceStruct");

    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    QQuickView* view = new QQuickView;
    view->setFormat(format);
    view->engine()->rootContext()->setContextProperty("jkInterface" ,&jki);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl(QLatin1String("qrc:/faroeVOP/MainWindow.qml")));
    view->connect(view->engine() ,&QQmlEngine::quit ,this ,&MainWindow::quit);
    ui->verticalLayout_2->addWidget(QWidget::createWindowContainer(view));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::quit()
{
//    close();
    qApp->quit();
}
