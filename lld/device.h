#ifndef DEVICE_H
#define DEVICE_H
#include "scanner.h"
#include "deviceio.h"
namespace JK {

class Device
{
public:
    Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp);
    virtual ~Device();
    int open(const char* );
    int open();
    int close();
    int scan();
    bool checkConnection();

    Scanner* getScanner();
private:
    DeviceIO* deviceIO;
    Scanner scanner;
};

}
#endif // DEVICE_H
