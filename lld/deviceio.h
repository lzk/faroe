#ifndef DEVICEIO_H
#define DEVICEIO_H
namespace JK {

class DeviceIO
{
public:
    enum{
        Type_usb,
        Type_net,
    };
    DeviceIO();
    virtual ~DeviceIO();
    virtual int type() = 0;
    virtual int open() = 0;
    virtual int close(void) = 0;
    virtual int write(char *buffer, int bufsize) = 0;
    virtual int read(char *buffer, int bufsize) = 0;
    virtual int resolveUrl(const char* url) = 0;
    virtual bool isConnected() = 0;
};

class NetDeviceIO:public DeviceIO
{
public:
    virtual int open(int port) = 0;
};

}
#endif // DEVICEIO_H
