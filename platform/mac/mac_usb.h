#ifndef MAC_USB_H
#define MAC_USB_H

#include <IOKit/usb/IOUSBLib.h>
#define IOUSBInterfaceInterface_version IOUSBInterfaceInterface245
#define IOUSBDeviceInterface_version IOUSBDeviceInterface245
#define kIOUSBInterfaceInterfaceID_version kIOUSBInterfaceInterfaceID245
#define kIOUSBDeviceInterfaceID_version kIOUSBDeviceInterfaceID245
typedef struct struct_deviceInfo{
    int locationID;
    int address;
    char serial[256];
}
    struct_deviceInfo;

typedef struct struct_deviceList{
    struct_deviceInfo* deviceList;
    int length;
    int maxLength;
}
    struct_deviceList;

typedef struct struct_device{
    struct_deviceInfo deviceInfo;
    IOUSBDeviceInterface_version** dev;
}
    struct_device;

typedef struct struct_deviceInterface{
    int interface;
    int inPipeRef;
    int outPipeRef;
    IOUSBInterfaceInterface_version **intf;
}
    struct_deviceInterface;
typedef void (* addUsbDevice)(struct_deviceInfo* ,void*);
typedef int (* usbDeviceHandler)(IOUSBDeviceInterface_version** ,void*);

int usb_getAddress(IOUSBDeviceInterface_version** dev);

int usb_getDeviceList(int vid ,int pid ,addUsbDevice addDevice ,void*);
int usb_getDeviceWithLocationID(int vid ,int pid ,int locationID ,usbDeviceHandler handler ,void*);
int usb_getDeviceWithSerial(int vid ,int pid ,const char* serial ,usbDeviceHandler handler ,void*);
int usb_getDeviceWithAddress(int vid ,int pid ,int address ,usbDeviceHandler handler ,void* pData);
int usb_open(IOUSBDeviceInterface_version **dev ,struct_deviceInterface* pDeviceInterface);
int usb_close(IOUSBDeviceInterface_version **dev ,IOUSBInterfaceInterface_version **intf);
int usb_writePipe(IOUSBInterfaceInterface_version **intf ,int outPipeRef ,char *buffer, size_t bufsize);
int usb_readPipe(IOUSBInterfaceInterface_version **intf ,int inPipeRef ,char *buffer, size_t bufsize);
int usb_readPipeAsync(IOUSBInterfaceInterface_version **intf ,int inPipeRef ,char *buffer, size_t bufsize);
int usb_intf_write(IOUSBInterfaceInterface_version **intf ,int interface ,char *buffer, size_t bufsize);
int usb_dev_write(IOUSBDeviceInterface_version **dev ,int interface ,char *buffer, size_t bufsize);
int usb_intf_read(IOUSBInterfaceInterface_version **intf ,int interface ,char *buffer, size_t bufsize);
int usb_dev_read(IOUSBDeviceInterface_version **dev ,int interface ,char *buffer, size_t bufsize);

static int nvram_read(IOUSBDeviceInterface_version** dev, unsigned char addr, unsigned int len, unsigned char *data);
static int nvram_readSerialNumber(IOUSBDeviceInterface_version **dev, char *serial);
#endif // MAC_USB_H
