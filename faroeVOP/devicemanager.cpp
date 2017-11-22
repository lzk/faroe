#include "devicemanager.h"
#include <QImage>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include "jkinterface.h"
#include "../lld/device.h"
#include "../lld/scanner.h"
#include "../lld/setter.h"
using namespace JK;
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

void DeviceManager::deviceCmd(int cmd)
{
    int err;
    void* data = NULL;
    switch (cmd) {
    case CMD_SCAN:
    {
        *device->getScanner()->getSetting() = jkInterface->parseUiScannerSetting();
//        data = (void*)device->getScanner()->getSetting();
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case CMD_setWifi:
    {
        Setter::struct_wifiSetting para = jkInterface->parseUiWifiSetting();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case CMD_setPassword:
    case CMD_confirmPassword:
    {
        QString para = jkInterface->parseUiPassword();
        data = (void*)para.toLatin1().data();
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case CMD_setSaveTime:
    {
        int para = jkInterface->parseUiSaveTime();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case CMD_getSaveTime:
    {
        int para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseSaveTime(para);
        break;
    }
    case CMD_getWifiInfo:
    {
        Setter::struct_wifiInfo para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseWifiInfo(para);
        break;
    }
    case CMD_getIpv4:
    {
        Setter::struct_ipv4 para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseIpv4(para);
        break;
    }
    case CMD_setIpv4:
    {
        Setter::struct_ipv4 para = jkInterface->parseUiIpv4();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case CMD_setSoftap:
    {
        Setter::struct_softAp para = jkInterface->parseUiSoftap();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case CMD_getSoftap:
    {
        Setter::struct_softAp para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseSoftap(para);
        break;
    }
    default:
        break;
    }
    emit cmdResult(cmd ,err);

}
