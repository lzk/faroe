#include "device.h"
#include "scanner.h"
#include "deviceio.h"
#include "setter.h"
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

int Device::open(const char* url)
{
    return deviceIO->open(url);
}

int Device::open()
{
    return deviceIO->open();
}


int Device::close()
{
    return deviceIO->close();
}

int Device::scan()
{
    int result = 0;
    if(deviceIO->type() == DeviceIO::Type_net){
//        result = deviceIO->read(&status ,1);
        result = ((NetDeviceIO*)deviceIO)->open(23011);
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
            result = scanner->ADFScan();
        }
        deviceIO->close();
    }
    return result;
}

bool Device::checkConnection()
{
    return deviceIO->isConnected();
}

int Device::deviceCmd(int cmd ,void* data)
{
    int err = ERR_cmd_cannot_support;
    switch (cmd) {
    case CMD_SCAN:
        err = scan();
        if(err)
            err += ERR_SCAN;
        break;
    case CMD_setWifi:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setWifi(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_setPassword:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setPassword(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_confirmPassword:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->confirmPassword(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_setSaveTime:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setSaveTime(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_getSaveTime:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getSaveTime(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_getWifiInfo:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getWifi(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_getIpv4:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->getIpV4(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_setIpv4:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setIpV4(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_setSoftap:
    {
        err =deviceIO->open();
        if(!err){
            err = setter->setSoftAp(data);
            deviceIO->close();
        }
        break;
    }
    case CMD_getSoftap:
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
