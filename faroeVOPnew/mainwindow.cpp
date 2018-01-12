#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
//    ,ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
//    delete ui;
}

void MainWindow::mmove(qreal x ,qreal y)
{
    move(this->x() + x ,this->y() + y);
}
