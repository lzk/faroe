#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void mmove(qreal x ,qreal y);

protected:
    bool event(QEvent *event);
private:
//    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
