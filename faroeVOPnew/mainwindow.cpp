#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
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

#include <QInputMethodQueryEvent>
bool MainWindow::event(QEvent *event)
{
    bool ret = QWidget::event(event);
//    qDebug()<<event->type();
    if(QEvent::InputMethodQuery == event->type()){
//         QInputMethodQueryEvent* inputEvent = (QInputMethodQueryEvent*)event;
//         qDebug() << inputEvent->value(Qt::ImQueryInput);
//         focusNextPrevChild(true);
//         focusNextPrevChild(false);
        qDebug()<<"widget focus:"<<hasFocus();
    }
    return ret;
}
