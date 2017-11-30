#ifndef USBIO_H
#define USBIO_H

#include "../../lld/deviceio.h"
#include "mac_usb.h"
using namespace JK;
class UsbIO : public DeviceIO
{
public:
    UsbIO();
    virtual int type();
    virtual int open();
    virtual int close(void);
    virtual int write(char *buffer, int bufsize);
    virtual int read(char *buffer, int bufsize);
    virtual int resolveUrl(const char* url);
    virtual bool isConnected();

public:
    void setDev(IOUSBDeviceInterface_version** dev);
    static int getUsbDeviceList(addUsbDevice addDevice ,void*);

private:
    int inPipeRef;
    int outPipeRef;
    int address;
    IOUSBInterfaceInterface_version **intf;
    IOUSBDeviceInterface_version** dev;
};

#endif // USBIO_H
