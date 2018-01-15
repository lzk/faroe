#include "devicemanager.h"
#include <QImage>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include "../newui/jkinterface.h"
#include "../newui/ImageViewer/imagemodel.h"
#include "../lld/device.h"
#include "../lld/scanner.h"
#include "../lld/setter.h"
#include "devicestruct.h"
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

void DeviceManager::initAfterMoveToThread()
{
    timerForDeviceWatcher = new QTimer(this);
    connect(timerForDeviceWatcher ,&QTimer::timeout ,this ,&DeviceManager::watchDevice);
    timerForDeviceWatcher->start(5000);
}

void DeviceManager::watchDevice()
{
    if(!device){
        QString url;
        QString currentDevice = jkInterface->getCurrentDevice();
        if(currentDevice.startsWith("USB")){
            url = QString("usb://") + "USB Device " + "?address=" + currentDevice.right(2);
#ifdef Q_OS_MAC
            device = new Device(url.toLatin1().data() ,&usbIO ,(PlatformApp*)&platformApp);
#endif
        }else if(!QHostAddress(currentDevice).isNull()){
            url = QString("socket://") + currentDevice;
            device = new Device(url.toLatin1().data() ,&netIO ,(PlatformApp*)&platformApp);
        }
    }
    if(device){
        emit updateDeviceStatus(device->checkConnection());
    }else{
        emit updateDeviceStatus(false);
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
        emit deviceConnected(m_deviceList[index]);
    }
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
        if(deviceInfo->type == DeviceInfo::Type_usb)
            m_deviceList << QString(deviceInfo->name) + QString(" ") + QString(deviceInfo->address);
        else
            m_deviceList << deviceInfo->address;

        emit updateDeviceList( m_deviceList);
    }
}

void DeviceManager::searchDeviceList()
{
    cancelSearch = 0;
    deviceList.clear();
    m_deviceList.clear();
    Device::searchDevices(addDevice ,this);
    QString currentDevice = jkInterface->getCurrentDevice();
    int index = 0;
    for (int i = 0; i < deviceList.count() ;i++) {
        if(deviceList[i].type == DeviceInfo::Type_usb){

        }else{

        }
        if(!currentDevice.compare(deviceList[i].address)){
            index = i;
            if(i != 0){
                m_deviceList.move(i ,0);
                emit updateDeviceList(m_deviceList);
            }
        }
    }
    if(index != 0)
        deviceList.move(index ,0);
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

#if TEST
bool g_cancelScan = false;
#endif
void DeviceManager::cancelScan()
{
#if TEST
    g_cancelScan = true;
#else
    if(device == NULL)
        return;
    Scanner* scanner = device->getScanner();
    scanner->cancel();
#endif
}

void DeviceManager::cmdToDevice(int cmd ,QString obj)
{
    qDebug()<<"cmd:"<<cmd;
    qDebug()<<"data:"<<obj;
    int err = 0;
    QString value = "";
#if TEST
    g_cancelScan = false;
    if(cmd == DeviceStruct::CMD_SCAN){
        QString sourceFile;
        for(int i = 1 ;i < 16 ;i++){
            if(g_cancelScan){
                err = DeviceStruct::ERR_SCAN_CANCEL;
                break;
            }
#ifdef Q_OS_MAC
            sourceFile = QString::asprintf("/Volumes/work/share/images/NatGeo%02d.jpg" ,i);
#elif Q_OS_WIN
            sourceFile = QString::asprintf("E:/tmp/pic/%d.jpg" ,i);
#endif
            saveImage(QImage(sourceFile));
            emit progressChanged(i / 15.0);
            QThread::sleep(1);
        }
    }
#else
    if(device == NULL){
        err = DeviceStruct::ERR_no_device;
        emit cmdResult(cmd ,err ,value);
        return;
    }
    void* data = NULL;
    switch (cmd) {
    case DeviceStruct::CMD_SCAN:
    {
        Scanner::Setting para = parseUiScannerSetting(obj);
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setWifi:
    {
        Setter::struct_wifiSetting para = parseUiWifiSetting(obj);
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setPassword:
    case DeviceStruct::CMD_confirmPassword:
    {
        QString para = parseUiPassword(obj);
        data = (void*)para.toLatin1().data();
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setSaveTime:
    {
        int para = parseUiSaveTime(obj);
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_getSaveTime:
    {
        int para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        value = uiParseSaveTime(para);
        break;
    }
    case DeviceStruct::CMD_getWifiInfo:
    {
        Setter::struct_wifiInfo para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        value = uiParseWifiInfo(para);
        break;
    }
    case DeviceStruct::CMD_getIpv4:
    {
        Setter::struct_ipv4 para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        value = uiParseIpv4(para);
        break;
    }
    case DeviceStruct::CMD_setIpv4:
    {
        Setter::struct_ipv4 para = parseUiIpv4(obj);
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_setSoftap:
    {
        Setter::struct_softAp para = parseUiSoftap(obj);
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        break;
    }
    case DeviceStruct::CMD_getSoftap:
    {
        Setter::struct_softAp para;
        data = (void*)&para;
        err = device->deviceCmd(cmd ,data);
        value = uiParseSoftap(para);
        break;
    }
    default:
        err = DeviceStruct::ERR_cmd_cannot_support;
        break;
    }
#endif
    emit cmdResult(cmd ,err ,value);
}

#include <QJsonDocument>
#include <QJsonObject>
Scanner::Setting DeviceManager::parseUiScannerSetting(const QString& obj)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(obj.toLatin1()).object();

    Scanner::Setting setting;
    setting.BitsPerPixel = jsonObj.value("colorMode").toBool(true) ?SETTING_IMG_24_BIT :SETTING_IMG_8_BIT;
    switch(jsonObj.value("dpi").toInt()){
    case 0:setting.dpi_x = setting.dpi_y = 200;break;
    case 1:setting.dpi_x = setting.dpi_y = 300;break;
    case 2:setting.dpi_x = setting.dpi_y = 600;break;
    default:setting.dpi_x = setting.dpi_y = 300;break;
    }
    switch (jsonObj.value("scanAreaSize").toInt()) {
    case 1://A4
        setting.scan_doc_size = SETTING_DOC_SIZE_A4;
        break;
    case 2://Letter
        setting.scan_doc_size = SETTING_DOC_SIZE_LT;
        break;
    case 0://auto
    default:
        setting.scan_doc_size = SETTING_DOC_SIZE_FULL;
        break;
    }

    setting.contrast = jsonObj.value("contrast").toInt(50);
    setting.brightness = jsonObj.value("brightness").toInt(50);
    setting.ADFMode = jsonObj.value("adfMode").toBool(true)? SETTING_SCAN_AB_SIDE : SETTING_SCAN_A_SIDE;
    setting.MultiFeed = jsonObj.value("multiFeed").toBool();
    setting.AutoCrop = jsonObj.value("autoCropAndDeskew").toBool();
    setting.onepage = SETTING_SCAN_PAGE;
    setting.source = SETTING_SCAN_ADF;
    setting.format = SETTING_IMG_FORMAT_JPG;
    return setting;
}


Setter::struct_wifiSetting DeviceManager::parseUiWifiSetting(const QString&)
{

}

QString DeviceManager::parseUiPassword(const QString&)
{

}

int DeviceManager::parseUiSaveTime(const QString&)
{

}

QString DeviceManager::uiParseSaveTime(int)
{

}

QString DeviceManager::uiParseWifiInfo(Setter::struct_wifiInfo)
{

}

QString DeviceManager::uiParseIpv4(Setter::struct_ipv4)
{

}

Setter::struct_ipv4 DeviceManager::parseUiIpv4(const QString&)
{

}

QString DeviceManager::uiParseSoftap(Setter::struct_softAp)
{

}

Setter::struct_softAp DeviceManager::parseUiSoftap(const QString&)
{

}
