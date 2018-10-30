#include "device.h"
#include "scanner.h"
#include "deviceio.h"
#include "setter.h"
#include "../platform/devicestruct.h"
#include "../platform/log.h"
using namespace JK;

Device::Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp)
    :deviceIO(dio)
    ,scanner(new Scanner)
    ,setter(new Setter)
{
    scanner->install(dio ,platformApp);
    setter->install(dio ,platformApp);
    resolveUrl(url);
}

Device::~Device()
{
    delete scanner;
    delete setter;
}

void Device::resolveUrl(const char* url)
{
    if(deviceIO){
        deviceIO->resolveUrl(url);
    }
    strcpy(currentUrl ,url);
}

Scanner* Device::getScanner()
{
    return scanner;
}

bool Device::checkConnection()
{
    return deviceIO->isConnected();
}

#ifdef OS_WIN
void Device::searchDevices(addDeviceHandler handler,void* pData)
{
}
#else
void snmpSearchDevices(addDeviceHandler handler,void* pData);
void usbSearchDevices(addDeviceHandler handler,void* pData);
void Device::searchUsbDevices(addDeviceHandler handler,void* pData)
{
    qDebug()<<"searching USB ...";
    usbSearchDevices(handler ,pData);
}

bool checkNetWorkStatus();
int Device::searchDevices(addDeviceHandler handler,void* pData)
{
    qDebug()<<"searching ...";
    usbSearchDevices(handler ,pData);
    int result = -1;
    if(checkNetWorkStatus()){
        snmpSearchDevices(handler ,pData);
        result = 0;
    }
    return result;
}
#endif
void snmpCancelSearch();
void Device::cancelSearch()
{
    snmpCancelSearch();
}

int Device::deviceCmd(int cmd ,void* data)
{
    int err = DeviceStruct::ERR_cmd_cannot_support;
    switch (cmd) {
    case CMD_SCAN:
        err = scanner->ADFScan(data);
        if(err > 0)
            err += DeviceStruct::ERR_SCAN;
        else if(err < 0){
            err = DeviceStruct::ERR_communication;
        }
        break;
    case CMD_doCalibration:
    {
        err = scanner->doCalibration();
        if(err > 0)
            err += DeviceStruct::ERR_SCAN;
        else if(err < 0){
            err = DeviceStruct::ERR_communication;
        }
        break;
    }
    case CMD_setWifi:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->setWifi(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_setPassword:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->setPassword(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_confirmPassword:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->confirmPassword(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_setSaveTime:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->setSaveTime(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_getSaveTime:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->getSaveTime(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_getWifiInfo:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->getWifi(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_getIpv4:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->getIpV4(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_setIpv4:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->setIpV4(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_setSoftap:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->setSoftAp(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_getSoftap:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->getSoftAp(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_setOffTime:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->setOffTime(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_getOffTime:
    {
//        err =deviceIO->open(1);
//        if(!err){
            err = setter->getOffTime(data);
//            deviceIO->close();
//        }
        break;
    }
    case CMD_setPowerSaveTime:
    {
        err = scanner->setPowerSaveTime(data);
        break;
    }
    case CMD_getDeviceSetting:
    {
        err = scanner->getDeviceSettings(data);
        break;
    }
    case CMD_getPowerSupply:
    {
        err = scanner->getPowerSupply(data);
        break;
    }
    case CMD_clearRollerCount:
    {
        err = scanner->clearRollerCount();
        break;
    }
    case CMD_clearACMCount:
    {
        err = scanner->clearACMCount();
        break;
    }
    default:
        break;
    }
    return err;
}
