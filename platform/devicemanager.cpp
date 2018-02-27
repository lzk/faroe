#include "devicemanager.h"
#include <QImage>
#include <QThread>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "../newui/jkinterface.h"
#include "../newui/ImageViewer/imagemodel.h"
#include "../lld/device.h"
#include "../lld/scanner.h"
#include "../lld/setter.h"
#include "devicestruct.h"
#include "platform.h"
DeviceManager::DeviceManager(QObject *parent)
    : jkInterface(static_cast<JKInterface*>(parent))
    ,device(NULL)
    ,cancelSearch(0)
{
    connect(&platformApp ,SIGNAL(addImage(QString)) ,this ,SLOT(addImage(QString)));
    connect(&platformApp ,SIGNAL(progressChanged(int ,int)) ,this ,SIGNAL(progressChanged(int ,int)));

    QTimer::singleShot(1000, this, SLOT(watchDevice()));
}

DeviceManager::~DeviceManager()
{
    if(device){
        delete device;
        device = NULL;
    }
}

void DeviceManager::watchDevice()
{
    QString currentDevice = jkInterface->getCurrentDevice();
    QString url;
    if(device){
        QString currentDeviceUrl = device->getCurrentUrl();
        if(currentDeviceUrl.startsWith("usb://")){
            if(!currentDevice.startsWith("USB") || currentDevice.right(2) != currentDeviceUrl.right(2)){
                device->resolveUrl(url.toLatin1().constData());
            }
        }else{
            if(!currentDeviceUrl.contains(currentDevice)){
                device->resolveUrl(url.toLatin1().constData());
            }
        }
    }else{
        if(currentDevice.startsWith("USB")){
            url = QString("usb://") + "USB Device " + "?address=" + currentDevice.right(2);
            device = new Device(url.toLatin1().data() ,&usbIO ,(PlatformApp*)&platformApp);
        }else if(!QHostAddress(currentDevice).isNull()){
            url = QString("socket://") + currentDevice;
            device = new Device(url.toLatin1().data() ,&netIO ,(PlatformApp*)&platformApp);
        }
    }

    bool connected = false;
    if(device){
        connected = device->checkConnection();
        if(!connected && currentDevice.startsWith("USB")){
            Device::searchUsbDevices(addUsbDevice ,this);
            connected = device->checkConnection();
        }
    }
    emit updateDeviceStatus(connected);
    QTimer::singleShot(5000, this, SLOT(watchDevice()));
}

void DeviceManager::connectDevice(int index)
{
    if(!deviceList.isEmpty() && (index >= 0) && deviceList.count() > index){
        DeviceInfo di = deviceList.at(index);
        connectDeviceInfo(&di);
    }
}

void DeviceManager::connectDeviceInfo(DeviceInfo* deviceInfo)
{
    if(!deviceInfo)
        return;
    if(device){
        delete device;
        device = NULL;
    }
    QString uiName;
    QString url;
    if(deviceInfo->type == DeviceInfo::Type_usb){
        url = QString("usb://") + deviceInfo->name + "?address=" + deviceInfo->address;
        device = new Device(url.toLatin1().data() ,&usbIO ,(PlatformApp*)&platformApp);
        uiName = QString(deviceInfo->name) + QString(" ") + QString(deviceInfo->address);
    }else{
        url = QString("socket://") + QString(deviceInfo->address);
        device = new Device(url.toLatin1().data() ,&netIO ,(PlatformApp*)&platformApp);
        uiName = deviceInfo->address;
    }
    emit deviceConnected(uiName);
    emit updateDeviceStatus(device->checkConnection());
}

void DeviceManager::addUsbDevice(DeviceInfo* deviceInfo ,void* pData)
{
    DeviceManager* dm = (DeviceManager*)pData;
    dm->connectDeviceInfo(deviceInfo);
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

void DeviceManager::addImage(QString filename)
{
    qDebug()<<"add new image:"<<filename;
    QSize sourceSize;
    QImage image;
    if(image.load(filename)){
        sourceSize = image.size();
    }
    if(!sourceSize.isValid()){
        qDebug()<<"not valid image!";
    }
    switch (currentCmd) {
    case DeviceStruct::CMD_ScanTo:{
        QString thumFilename = ImageModel::getThumbnailFilename(filename);
        image.scaledToWidth(100).save(thumFilename);
        emit scanedImage(filename ,sourceSize);
        break;
    }
    case DeviceStruct::CMD_SCAN:
        emit scanedImage(filename ,sourceSize);
        break;
    default:
        break;
    }
}

#if TEST
bool g_cancelScan = false;
void DeviceManager::testAddImage(const QString& sourceFile)
{
    QImage image(sourceFile);
    if(image.isNull())
        return;
    QString tmppath = getTempPath();
    QString tmpFile;
    tmpFile = tmppath + "/tmpscaniamge_" + QDateTime::currentDateTime().toString("yyMMddHHmmsszzz") +".jpg";
    image.save(tmpFile);

    addImage(tmpFile);
}
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
    int err = 0;
    QString value = "";
//    qDebug()<<"cmd:"<<cmd;
//    qDebug()<<"data:"<<obj;
#if TEST
    g_cancelScan = false;
    switch (cmd) {
    case DeviceStruct::CMD_ScanTo:
    case DeviceStruct::CMD_SCAN:
    {
        currentCmd = cmd;
        QString sourceFile;
        emit progressChanged(DeviceStruct::ScanningProgress_Start ,0);
        for(int i = 1 ;i < 4 ;i++){
            if(g_cancelScan){
                err = DeviceStruct::ERR_RETSCAN_CANCEL;
                break;
            }
            emit progressChanged(DeviceStruct::ScanningProgress_Upload ,i);
#ifdef Q_OS_MAC
            sourceFile = QString::asprintf("/Volumes/work/share/images/NatGeo%02d.jpg" ,i);
#endif
#ifdef Q_OS_WIN
            sourceFile = QString::asprintf("E:/tmp/pic/%d.jpg" ,i);
#endif
            testAddImage(sourceFile);
            QThread::sleep(1);
            emit progressChanged(DeviceStruct::ScanningProgress_Completed ,i);
            QThread::sleep(1);
        }
        break;
    }
    default:
        break;
    }
    emit cmdResult(cmd ,err ,value);
    return;
#endif

    if(device == NULL){
        err = DeviceStruct::ERR_no_device;
        emit cmdResult(cmd ,err ,value);
        return;
    }
    void* data = NULL;
    switch (cmd) {
    case DeviceStruct::CMD_ScanTo:
    case DeviceStruct::CMD_SCAN:
    {
        currentCmd = cmd;
        Scanner::Setting para = parseUiScannerSetting(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_SCAN ,data);
        break;
    }
    case DeviceStruct::CMD_setWifi:
    {
        Setter::struct_wifiSetting para = parseUiWifiSetting(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_setWifi ,data);
        break;
    }
    case DeviceStruct::CMD_setPassword:
    {
        QString para = parseUiPassword(obj);
        data = (void*)para.toLatin1().data();
        err = device->deviceCmd(Device::CMD_setPassword ,data);
        break;
    }
    case DeviceStruct::CMD_confirmPassword:
    {
        QString para = parseUiPassword(obj);
        data = (void*)para.toLatin1().data();
        err = device->deviceCmd(Device::CMD_confirmPassword ,data);
        break;
    }
    case DeviceStruct::CMD_setSaveTime:
    {
        int para = parseUiSaveTime(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_setSaveTime ,data);
        break;
    }
    case DeviceStruct::CMD_getSaveTime:
    {
        int para;
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_getSaveTime ,data);
        value = uiParseSaveTime(para);
        break;
    }
    case DeviceStruct::CMD_setOffTime:
    {
        int para = parseUiOffTime(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_setOffTime ,data);
        break;
    }
    case DeviceStruct::CMD_getOffTime:
    {
        int para;
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_getOffTime ,data);
        value = uiParseOffTime(para);
        break;
    }
    case DeviceStruct::CMD_setDeviceSetting:
    {
        Setter::struct_deviceSetting para = parseUiDeviceSetting(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_setDeviceSetting ,data);
        break;
    }
    case DeviceStruct::CMD_getDeviceSetting:
    {
        Setter::struct_deviceSetting para;
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_getDeviceSetting ,data);
        value = uiParseDeviceSetting(para);
        break;
    }
    case DeviceStruct::CMD_getWifiInfo:
    {
        Setter::struct_wifiInfo para;
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_getWifiInfo ,data);
        value = uiParseWifiInfo(para);
        break;
    }
    case DeviceStruct::CMD_getIpv4:
    {
        Setter::struct_ipv4 para;
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_getIpv4 ,data);
        value = uiParseIpv4(para);
        break;
    }
    case DeviceStruct::CMD_setIpv4:
    {
        Setter::struct_ipv4 para = parseUiIpv4(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_setIpv4 ,data);
        break;
    }
    case DeviceStruct::CMD_setSoftap:
    {
        Setter::struct_softAp para = parseUiSoftap(obj);
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_setSoftap ,data);
        break;
    }
    case DeviceStruct::CMD_getSoftap:
    {
        Setter::struct_softAp para;
        data = (void*)&para;
        err = device->deviceCmd(Device::CMD_getSoftap ,data);
        value = uiParseSoftap(para);
        break;
    }
    default:
        err = DeviceStruct::ERR_cmd_cannot_support;
        break;
    }
    emit cmdResult(cmd ,err ,value);
}

Scanner::Setting DeviceManager::parseUiScannerSetting(const QString& obj)
{
    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    QJsonObject jsonScanSetting = jsonobj.value("scanSetting").toObject();
    Scanner::Setting setting;
    setting.BitsPerPixel = jsonScanSetting.value("colorMode").toBool(true) ?SETTING_IMG_24_BIT :SETTING_IMG_8_BIT;
    switch(jsonScanSetting.value("dpi").toInt()){
    case 0:setting.dpi_x = setting.dpi_y = 150;break;
    case 1:setting.dpi_x = setting.dpi_y = 200;break;
    case 2:setting.dpi_x = setting.dpi_y = 300;break;
    case 3:setting.dpi_x = setting.dpi_y = 600;break;
    default:setting.dpi_x = setting.dpi_y = 200;break;
    }
    setting.scan_doc_size = jsonScanSetting.value("scanAreaSize").toInt();
    if(setting.scan_doc_size == -1)
        setting.scan_doc_size = 0;
//    switch (jsonScanSetting.value("scanAreaSize").toInt()) {
//    case 1://A4
//        setting.scan_doc_size = SETTING_DOC_SIZE_A4;
//        break;
//    case 2://Letter
//        setting.scan_doc_size = SETTING_DOC_SIZE_LT;
//        break;
//    case 0://auto
//    default:
//        setting.scan_doc_size = SETTING_DOC_SIZE_FULL;
//        break;
//    }

    setting.contrast = jsonScanSetting.value("contrast").toInt(50);
    setting.brightness = jsonScanSetting.value("brightness").toInt(50);
    setting.ADFMode = jsonScanSetting.value("adfMode").toBool()? SETTING_SCAN_AB_SIDE : SETTING_SCAN_A_SIDE;
    setting.MultiFeed = jsonScanSetting.value("multiFeed").toBool();
    setting.AutoCrop = jsonScanSetting.value("autoCropAndDeskew").toBool();
    setting.onepage = false;//SETTING_SCAN_PAGE;
    setting.source = SETTING_SCAN_ADF;
    setting.format = SETTING_IMG_FORMAT_JPG;

    setting.bColorDetect = jsonScanSetting.value("autoColorDetection").toBool();
    setting.bSkipBlankPage = jsonScanSetting.value("skipBlankPage").toBool();
    setting.gammaValue = jsonScanSetting.value("gamma").toInt();
    setting.type = jsonScanSetting.value("mediaType").toInt();
    return setting;
}


Setter::struct_wifiSetting DeviceManager::parseUiWifiSetting(const QString& obj)
{

    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    Setter::struct_wifiSetting para;
    para.channel = jsonobj.value("channel").toInt(0);
    para.enable = jsonobj.value("enable").toBool();
    para.encryption = jsonobj.value("encryption").toInt(0);
    para.type = jsonobj.value("type").toInt(0);
    para.wepKeyId = jsonobj.value("wepKeyId").toInt(0);
    QString string;
    int count;
    const char* str;
    string = jsonobj.value("ssid").toString();
    count = string.count();
    str = string.toLatin1().constData();
    memset(para.ssid ,0 ,32);
    if(count >= 32){
        memcpy(para.ssid ,str ,32);
    }else{
        strcpy(para.ssid ,str);
    }

    string = jsonobj.value("password").toString();
    count = string.count();
    str = string.toLatin1().constData();
    memset(para.password ,0 ,64);
    if(count >= 64){
        memcpy(para.password ,str ,64);
    }else{
        strcpy(para.password ,str);
    }
    return para;
}

QString DeviceManager::uiParseWifiInfo(Setter::struct_wifiInfo wifiInfo)
{
    QJsonArray jarray;
    for(int i = 0;i < 10 ;i++){
        QString ssid(wifiInfo.aplist[i].ssid);
        if(ssid.isEmpty()){
            break;
        }
        jarray << QJsonObject {
        {"ssid",ssid}
        ,{"encryption" ,wifiInfo.aplist[i].encryption}
        };
    }
    char ssid[33];
    memset(ssid ,0 ,33);
    memcpy(ssid ,wifiInfo.wifiSetting.ssid ,32);
    char password[65];
    memset(password ,0 ,65);
    memcpy(password ,wifiInfo.wifiSetting.password ,64);
    QJsonObject obj{
        {"enable",wifiInfo.wifiSetting.enable}
        ,{"type",wifiInfo.wifiSetting.type}
        ,{"encryption",wifiInfo.wifiSetting.encryption}
        ,{"wepKeyId",wifiInfo.wifiSetting.wepKeyId}
        ,{"channel",wifiInfo.wifiSetting.channel}
        ,{"ssid",QString(ssid)}
        ,{"password",QString(password)}
        ,{"apList",jarray}
    };

    return QString(QJsonDocument(obj).toJson());
}

QString DeviceManager::parseUiPassword(const QString&)
{

}

int DeviceManager::parseUiSaveTime(const QString& obj)
{

    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    int para;
    para = jsonobj.value("saveTime").toInt(0);
    return para;
}

QString DeviceManager::uiParseSaveTime(int para)
{
    QJsonObject obj{
        {"saveTime",para}
    };

    return QString(QJsonDocument(obj).toJson());
}
int DeviceManager::parseUiOffTime(const QString& obj)
{
    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    int para;
    para = jsonobj.value("offTime").toInt(0);
    return para;
}

QString DeviceManager::uiParseOffTime(int para)
{
    QJsonObject obj{
        {"offTime",para}
    };

    return QString(QJsonDocument(obj).toJson());
}

Setter::struct_deviceSetting DeviceManager::parseUiDeviceSetting(const QString& obj)
{

    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    Setter::struct_deviceSetting para;
    para.saveTime = jsonobj.value("saveTime").toInt(0);
    para.offTime = jsonobj.value("offTime").toInt(0);
    return para;
}

QString DeviceManager::uiParseDeviceSetting(Setter::struct_deviceSetting para)
{
    QJsonObject obj{
        {"saveTime",para.saveTime}
        ,{"offTime",para.offTime}
    };

    return QString(QJsonDocument(obj).toJson());
}

QString DeviceManager::uiParseIpv4(Setter::struct_ipv4 para)
{
    QString address = QString().sprintf("%d.%d.%d.%d"
                ,para.address[0] ,para.address[1] ,para.address[2] ,para.address[3]);
    QString subnetMask = QString().sprintf("%d.%d.%d.%d"
                ,para.subnetMask[0] ,para.subnetMask[1] ,para.subnetMask[2] ,para.subnetMask[3]);
    QString gatewayAddress = QString().sprintf("%d.%d.%d.%d"
                ,para.gatewayAddress[0] ,para.gatewayAddress[1] ,para.gatewayAddress[2] ,para.gatewayAddress[3]);
    QJsonObject obj{
        {"address",address}
        ,{"addressMode",para.addressMode}
        ,{"gatewayAddress",gatewayAddress}
        ,{"mode",para.mode}
        ,{"subnetMask",subnetMask}
    };

    return QString(QJsonDocument(obj).toJson());
}

#include <QHostAddress>
Setter::struct_ipv4 DeviceManager::parseUiIpv4(const QString& obj)
{
    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    Setter::struct_ipv4 para;
    int address = QHostAddress(jsonobj.value("address").toString()).toIPv4Address();
    para.address[0] = (address & 0xff000000) >> 24;
    para.address[1] = (address & 0x00ff0000) >> 16;
    para.address[2] = (address & 0x0000ff00) >> 8;
    para.address[3] = (address & 0x000000ff);
    address = QHostAddress(jsonobj.value("subnetMask").toString()).toIPv4Address();
    para.subnetMask[0] = (address & 0xff000000) >> 24;
    para.subnetMask[1] = (address & 0x00ff0000) >> 16;
    para.subnetMask[2] = (address & 0x0000ff00) >> 8;
    para.subnetMask[3] = (address & 0x000000ff);
    address = QHostAddress(jsonobj.value("gatewayAddress").toString()).toIPv4Address();
    para.gatewayAddress[0] = (address & 0xff000000) >> 24;
    para.gatewayAddress[1] = (address & 0x00ff0000) >> 16;
    para.gatewayAddress[2] = (address & 0x0000ff00) >> 8;
    para.gatewayAddress[3] = address & 0x000000ff;
    para.mode = jsonobj.value("mode").toInt(1);
    para.addressMode = jsonobj.value("addressMode").toInt(0);
    return para;
}

QString DeviceManager::uiParseSoftap(Setter::struct_softAp para)
{
    char ssid[33];
    memset(ssid ,0 ,33);
    memcpy(ssid ,para.ssid ,32);
    char password[65];
    memset(password ,0 ,65);
    memcpy(password ,para.password ,64);
    QJsonObject obj{
        {"enable",para.enable}
        ,{"ssid",QString(ssid)}
        ,{"password",QString(password)}
    };

//    QJsonObject obj{
//        {"enable",para.enable}
//        ,{"ssid",para.ssid}
//        ,{"password",para.password}
//    };
    return QString(QJsonDocument(obj).toJson());
}

Setter::struct_softAp DeviceManager::parseUiSoftap(const QString& obj)
{
    QJsonObject jsonobj = QJsonDocument::fromJson(obj.toLatin1()).object();
    Setter::struct_softAp para;
    para.enable = jsonobj.value("enable").toBool();
    QString string;
    int count;
    const char* str;
    string = jsonobj.value("ssid").toString();
    count = string.count();
    str = string.toLatin1().constData();
    memset(para.ssid ,0 ,32);
    if(count >= 32){
        memcpy(para.ssid ,str ,32);
    }else{
        strcpy(para.ssid ,str);
    }

    string = jsonobj.value("password").toString();
    count = string.count();
    str = string.toLatin1().constData();
    memset(para.password ,0 ,64);
    if(count >= 64){
        memcpy(para.password ,str ,64);
    }else{
        strcpy(para.password ,str);
    }
    return para;
}
