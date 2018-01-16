#ifndef JKINTERFACE_H
#define JKINTERFACE_H

#include <QObject>
#include <QSize>
#include <QMutex>
#include <QThread>
#include <QJSValue>
class DeviceManager;
class ImageModel;
class ImageManager;
class JKInterface : public QObject
{
    Q_OBJECT
public:
    explicit JKInterface(QObject *parent = nullptr);
    ~JKInterface();
    void setScanDataHandle(QObject* scanData);
    QString getCurrentDevice();
    void installImageModel(ImageModel* imageModel){this->imageModel=imageModel;}

public:
    Q_INVOKABLE QStringList getPrinterName();
    Q_INVOKABLE void cancelScan();

    Q_INVOKABLE void toPrint(QList<int>);
    Q_INVOKABLE void toEmail(QList<int> ,int);
    Q_INVOKABLE void toFile(QList<int> ,const QString&);
    Q_INVOKABLE void toFTP(QList<int> ,const QString& jsonData);
    Q_INVOKABLE void toApplication(QList<int> ,const QString&);
    Q_INVOKABLE void toCloud(QList<int> ,const QString& jsonData);
signals:
    void searchDeviceList();
//    void cancelSearch();
    void searchComplete();
    void connectDevice(int);

    void cmdToDevice(int cmd ,QString data);
    void cmdResult(int cmd,int result ,QString data);

//    void progressChanged();
//    void ADFModeChanged();
//    void dpiChanged();
//    void colorModeChanged();
//    void scanAreaSizeChanged();
//    void MultiFeedChanged();
//    void contrastChanged();
//    void brightnessChanged();
//    void AutoCropAndDeskewChanged();

    void imagesToPrint(QStringList ,QString);
    void imagesToEmail(QStringList ,int);
    void imagesToFile(QStringList ,QString);
    void imagesToFTP(QStringList ,QString);
    void imagesToApplication(QStringList ,QString);
    void imagesToCloud(QStringList ,QString);
    void ftpUploadComplete();


public slots:
//    void setProgress(qreal prgrs);
    void addImage(QString filename,QSize sourceSize);
    void updateDeviceList(QStringList);
    void deviceConnected(QString);
    void updateDeviceStatus(bool);

private:
    QObject* scanData;
    DeviceManager* deviceManager;
    ImageManager* imageManager;
    ImageModel* imageModel;
    QMutex mutex;
    QThread thread;
};

#endif // JKINTERFACE_H
