#include "devicemanager.h"
#include <QImage>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include "jkinterface.h"
#include "../lld/device.h"
#include "../lld/scanner.h"
#include "../lld/setter.h"
#include "devicestruct.h"
#define TEST 0
DeviceManager::DeviceManager(QObject *parent)
    : jkInterface(static_cast<JKInterface*>(parent))
    ,device(NULL)
    ,cancelSearch(0)
{
    connect(&platformApp ,SIGNAL(addImage(QString ,QSize)) ,this ,SIGNAL(addImage(QString ,QSize)));
    connect(&platformApp ,SIGNAL(progressChanged(qreal)) ,this ,SIGNAL(progressChanged(qreal)));
}
DeviceManager::~DeviceManager()
{
    if(device){
        delete device;
        device = NULL;
    }
}

void DeviceManager::connectDevice(int index)
{
    if(device){
        delete device;
        device = NULL;
    }
    if(!deviceList.isEmpty() && (index >= 0) && deviceList.count() > index){
        QString url;
        if(deviceList.at(index).type == DeviceInfo::Type_usb){
            url = QString("usb://") + deviceList.at(index).name + "?address=" + deviceList.at(index).address;
#ifdef Q_OS_MAC
            device = new Device(url.toLatin1().data() ,&usbIO ,(PlatformApp*)&platformApp);
#endif
        }else{
            url = QString("socket://") + QString(deviceList.at(index).address);
            device = new Device(url.toLatin1().data() ,&netIO ,(PlatformApp*)&platformApp);
        }
    }
}

void DeviceManager::cancelScan()
{
    if(device == NULL)
        return;
    Scanner* scanner = device->getScanner();
    scanner->cancel();
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

void DeviceManager::addDevice(DeviceInfo* deviceInfo ,void* pData)
{
    DeviceManager* dm = (DeviceManager*)pData;
    dm->addDeviceInfo(deviceInfo ,1);
}

void DeviceManager::addDeviceInfo(DeviceInfo* deviceInfo ,int count)
{
    for(int i = 0 ;i < count ;i++){
        deviceList << deviceInfo[i];
        emit addDeviceInfo(QString(deviceInfo[i].name) + "(" + QString(deviceInfo->address) + ")");
    }
}

void DeviceManager::searchDeviceList()
{
    cancelSearch = 0;
    deviceList.clear();
    Device::searchDevices(addDevice ,this);
    emit searchComplete();
}

void DeviceManager::cancelSearchDeviceList()
{
    cancelSearch = 1;
}

int DeviceManager::isCancelSearch()
{
    return cancelSearch;
}

void DeviceManager::deviceCmd(int cmd)
{
    int err = 0;
#if TEST
    if(cmd == DeviceStruct::CMD_SCAN){
        QString sourceFile;
        for(int i = 1 ;i < 16 ;i++){
            sourceFile = QString::asprintf("E:/tmp/pic/%d.jpg" ,i);
            saveImage(QImage(sourceFile));
            emit progressChanged(i / 15.0);
            QThread::sleep(1);
        }
    }
#else
    if(device == NULL){
        err = DeviceStruct::ERR_no_device;
        emit cmdResult(cmd ,err);
        return;
    }
    void* data = NULL;
    switch (cmd) {
    case DeviceStruct::CMD_SCAN:
    {
        *device->getScanner()->getSetting() = jkInterface->parseUiScannerSetting();
//        data = (void*)device->getScanner()->getSetting();
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setWifi:
    {
        Setter::struct_wifiSetting para = jkInterface->parseUiWifiSetting();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setPassword:
    case DeviceStruct::CMD_confirmPassword:
    {
        QString para = jkInterface->parseUiPassword();
        data = (void*)para.toLatin1().data();
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setSaveTime:
    {
        int para = jkInterface->parseUiSaveTime();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_getSaveTime:
    {
        int para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseSaveTime(para);
        break;
    }
    case DeviceStruct::CMD_getWifiInfo:
    {
        Setter::struct_wifiInfo para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseWifiInfo(para);
        break;
    }
    case DeviceStruct::CMD_getIpv4:
    {
        Setter::struct_ipv4 para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseIpv4(para);
        break;
    }
    case DeviceStruct::CMD_setIpv4:
    {
        Setter::struct_ipv4 para = jkInterface->parseUiIpv4();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setSoftap:
    {
        Setter::struct_softAp para = jkInterface->parseUiSoftap();
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_getSoftap:
    {
        Setter::struct_softAp para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        jkInterface->uiParseSoftap(para);
        break;
    }
    default:
        err = DeviceStruct::ERR_cmd_cannot_support;
        break;
    }
#endif
    emit cmdResult(cmd ,err);

}
