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
    if(deviceIO){
        deviceIO->resolveUrl(url);
    }
}

Device::~Device()
{
    delete scanner;
    delete setter;
}

Scanner* Device::getScanner()
{
    return scanner;
}

int Device::open()
{
    return deviceIO->open();
}


int Device::close()
{
    return deviceIO->close();
}

int Device::scan(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    Scanner::Setting* setting = (Scanner::Setting*)data;
    int result = 0;
    if(deviceIO->type() == DeviceIO::Type_net){
//        result = deviceIO->read(&status ,1);
        result = ((NetDeviceIO*)deviceIO)->openPort(23011);
        if(!result){
            char cmd[4] = { 'J','D','G','S' };
            char status[8] = { 0 };
            if(4 == deviceIO->write(cmd ,4)){
                if(8==deviceIO->read(status ,8)){
                    if (   status[0] == 'J'
                        && status[1] == 'D'
                        && status[2] == 'A'
                        && status[4] == 0x00){
                        result = RETSCAN_OK;
                    }else{
                        result = RETSCAN_BUSY;
                    }
                }
            }
            deviceIO->close();
        }
    }
    if(result != RETSCAN_OK)
        return result;
    result = deviceIO->open();
    if(!result){
        char status = 1;
        if(deviceIO->type() == DeviceIO::Type_net){
            result = deviceIO->read(&status ,1);
        }
        if(status == 1){
            result = scanner->ADFScan(setting);
        }
        deviceIO->close();
    }
    return result;
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
void Device::searchDevices(addDeviceHandler handler,void* pData)
{
    qDebug()<<"searching ...";
    usbSearchDevices(handler ,pData);
    snmpSearchDevices(handler ,pData);
}
#endif
int Device::deviceCmd(int cmd ,void* data)
{
    int err = DeviceStruct::ERR_cmd_cannot_support;
    switch (cmd) {
    case DeviceStruct::CMD_SCAN:
        err = scan(data);
        if(err)
            err += DeviceStruct::ERR_SCAN;
        break;
    case DeviceStruct::CMD_setWifi:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setWifi(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_setPassword:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setPassword(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_confirmPassword:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->confirmPassword(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_setSaveTime:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setSaveTime(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_getSaveTime:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getSaveTime(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_getWifiInfo:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getWifi(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_getIpv4:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getIpV4(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_setIpv4:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setIpV4(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_setSoftap:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setSoftAp(data);
            deviceIO->close();
        }
        break;
    }
    case DeviceStruct::CMD_getSoftap:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getSoftAp(data);
            deviceIO->close();
        }
        break;
    }
    default:
        break;
    }
    return err;
}
