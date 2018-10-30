#ifndef JKINTERFACE_H
#define JKINTERFACE_H

#include <QObject>
#include <QSize>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QValidator>
class DeviceManager;
class ImageModel;
class ImageManager;
class Worker;
class JKInterface : public QObject
{
    Q_OBJECT
public:
    explicit JKInterface(QObject *parent = nullptr);
    ~JKInterface();

public:
    QString getCurrentDevice();
    void installImageModel(ImageModel* imageModel){this->imageModel=imageModel;}
public:
    Q_INVOKABLE QStringList getPrinterName();
    Q_INVOKABLE QString getDefaultPrinterName();
    Q_INVOKABLE void cancelScan();
    Q_INVOKABLE void cancelSearch();
    Q_INVOKABLE void cancelImageHandle();
    Q_INVOKABLE void setCmd(int cmd ,const QString& data=QString(),QStringList fileList=QStringList());

    Q_INVOKABLE bool pathExist(const QString& filePath);
    Q_INVOKABLE void setScanDataHandle(QObject* scanData);
    Q_INVOKABLE QString homeDictory();
    Q_INVOKABLE void showMinimize(QWindow*);
    Q_INVOKABLE void setWindowFrameless(QWindow*);
    Q_INVOKABLE int macVersion();
    Q_INVOKABLE bool validate(const QValidator* ,QString& text);
    Q_INVOKABLE bool isSubmask(const QString& submask);
    Q_INVOKABLE bool isIpv4(const QString& ip);
signals:
    void searchDeviceList();
    void searchComplete(int result);
    void connectDevice(int);

    void progressChanged(int progress ,int page);

    void init(const QString&);
    void deviceConnectCompleted();

    void cancel(bool);
    void cmdToWorker(int cmd ,QString data ,QStringList fileList=QStringList());
    void cmdResultToUi(int cmd ,QString data=QString() ,int result=0 ,int resultType=0 ,int phase = 0);

public slots:
    void scanedImage(QString filename,QSize sourceSize);
    void updateDeviceList(QStringList);
    void deviceConnected(QString);
    void updateDeviceStatus(bool);

private:

private:
    QObject* scanData;
    ImageModel* imageModel;
    Worker* worker;
    QStringList printerNameList;
    QMutex mutex;
    QThread thread;
    QThread thread_decode;
};

#endif // JKINTERFACE_H
