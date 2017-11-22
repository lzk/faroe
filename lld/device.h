#ifndef DEVICE_H
#define DEVICE_H
#include "scanner.h"
#include "deviceio.h"
class DeviceManager;
namespace JK {

typedef struct DeviceInfo{
    typedef enum{
        Type_usb,
        Type_ipv4,
        Type_ipv6,
    }
        Type;
    Type type;
    char name[256];
    char address[256];
}
    DeviceInfo;

typedef void (*addDeviceHandler)(DeviceInfo* ,void*);
class Device
{
public:
    Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp);
    virtual ~Device();
    int open();
    int close();
    int scan();
    bool checkConnection();

    Scanner* getScanner();

    static void searchDevices(addDeviceHandler handler,void* pData);
private:
    DeviceIO* deviceIO;
    Scanner scanner;
};

}
#endif // DEVICE_H
