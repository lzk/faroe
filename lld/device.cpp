#include "device.h"
using namespace JK;

Device::Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp)
    :deviceIO(dio)
{
    scanner.install(dio ,platformApp);
    if(deviceIO){
        deviceIO->resolveUrl(url);
    }
}

Device::~Device()
{
}

Scanner* Device::getScanner()
{
    return &scanner;
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
            result = scanner.ADFScan();
        }
        deviceIO->close();
    }
    return result;
}

bool Device::checkConnection()
{
    return deviceIO->isConnected();
}
