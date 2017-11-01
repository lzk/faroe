#include "devicemanager.h"
#include <QImage>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include "jkinterface.h"
#include "../lld/device.h"
#define TEST 0
DeviceManager::DeviceManager(QObject *parent)
    : jkInterface(static_cast<JKInterface*>(parent))
    ,device(NULL)
{
    connect(&platformApp ,SIGNAL(addImage(QString ,QSize)) ,this ,SIGNAL(addImage(QString ,QSize)));
    connect(&platformApp ,SIGNAL(progressChanged(qreal)) ,this ,SIGNAL(progressChanged(qreal)));
    resolveUrl("socket://192.168.6.102");
//    resolveUrl("socket://192.168.186.1");
}
DeviceManager::~DeviceManager()
{
    if(device){
        delete device;
        device = NULL;
    }
}

void DeviceManager::resolveUrl(const QString& url)
{
    if(device){
        delete device;
        device = NULL;
    }
    if(url.startsWith("usb://")){
//        device = new Device;
//        device->install(NULL);
    }else if(url.startsWith("socket://")){
        device = new Device(url.toLatin1().data() ,&netIO ,(PlatformApp*)&platformApp);
    }
}

#if TEST
static bool mcancel = false;
#endif
void DeviceManager::cancelScan()
{
#if TEST
    mcancel = true;
#else
    if(device == NULL)
        return;
    Scanner* scanner = device->getScanner();
    scanner->cancel();
#endif
}

void DeviceManager::saveImage(const QImage& image)
{
    if(image.isNull())
        return;
    QString tmpFile;
    QString thumFile;
    QSize sourceSize;
    tmpFile = platformApp.tmppath + "/tmpscaniamge_" + QDateTime::currentDateTime().toString("yyMMddHHmmsszzz") +".jpg";
    thumFile = ImageModel::getThumbnailFilename(tmpFile);

    image.save(tmpFile);
    sourceSize = image.size();
    image.scaled(100 ,100).save(thumFile);
    emit addImage(tmpFile ,sourceSize);
}

#define NO_DEVICE -10
void DeviceManager::scan()
{
    int result = 0;
#if TEST
    mcancel = false;
    QString sourceFile;
    for(int i = 1 ;i < 16 ;i++){
        if(mcancel){
            result = 30;
            break;
        }
        sourceFile = QString::asprintf("E:/tmp/pic/%d.jpg" ,i);
        saveImage(QImage(sourceFile));
        emit progressChanged(i / 15.0);
        QThread::sleep(1);
    }
#else
    if(device == NULL){
        result = NO_DEVICE;
    }else{
        *device->getScanner()->getSetting() = jkInterface->parseUiScannerSetting();
        result = device->scan();
    }
#endif
    emit scanResult(result);
}
