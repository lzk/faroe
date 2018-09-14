#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QObject>
#include <QSize>
#include <QMutex>
class JKInterface;
class DeviceManager;
class ImageFunctions;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

signals:
    void cmdToDevice(int cmd ,QString data);
    void cmdResult(int cmd,int result ,QString data=QString());
    void cmdResultToUi(int cmd ,QString data=QString() ,int result=0 ,int resultType=0 ,int phase = 0);

public slots:
    void cmdFromUi(int cmd ,QString data ,QStringList fileList=QStringList());
    void deviceCmdResult(int cmd,int result ,QString data);
    void deviceCmdResult_immediately(int cmd,int result ,QString data);
    void scanedImage(QString filename,QSize sourceSize);

private slots:
    void cancel(bool);
private:
    JKInterface* jkInterface;
    DeviceManager* deviceManager;
    ImageFunctions* imageFunctions;

    int cmd;
    int cmd_status;
    int err;
    QString para;
    QString result;
    QThread thread;
    QMutex mutex;

    void cmdComplete(int cmd ,QString data=QString() ,int result=0 ,int resultType=0);
};

#endif // WORKER_H
