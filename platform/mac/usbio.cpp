#include "usbio.h"
#include "mac_usb.h"
#include "../../lld/device.h"
#include <string.h>
#include <QUrlQuery>
#include "../log.h"
#define USB_VID 0x05e3
#define USB_PID 0x0118
typedef struct struct_searchData{
    addDeviceHandler handler;
    void* pData;
}
    struct_searchData;

static void addDeviceInfo(struct_deviceInfo* pDeviceInfo ,void* pData)
{
    DeviceInfo di;
    di.type = DeviceInfo::Type_usb;
//    sprintf(di.address ,"%02d" ,pDeviceInfo->address);
    strcpy(di.address ,pDeviceInfo->serial);
    sprintf(di.name ,"USB Device %s" ,di.address);
    LOG_PARA("usb serial:%s" ,pDeviceInfo->serial);
    LOG_PARA("usb locationID:%d" ,pDeviceInfo->locationID);
    LOG_PARA("usb address:%d" ,pDeviceInfo->address);
    struct_searchData* psd = (struct_searchData*)pData;
    if(psd->handler)
        psd->handler(&di ,psd->pData);
}

void usbSearchDevices(addDeviceHandler handler,void* pData)
{
    struct_searchData sd;
    sd.handler = handler;
    sd.pData = pData;
    usb_getDeviceList(USB_VID ,USB_PID ,addDeviceInfo ,&sd);
}

UsbIO::UsbIO() :
    inPipeRef(0)
  ,outPipeRef(0)
  ,address(-1)
  ,intf(NULL)
  ,dev(NULL)
{

}

void UsbIO::setDev(IOUSBDeviceInterface_version** dev)
{
    this->dev = dev;
}

int UsbIO::type()
{
    return DeviceIO::Type_usb;
}

int devHandler(IOUSBDeviceInterface_version** dev,void* pData)
{
    UsbIO* io = (UsbIO*)pData;
    io->setDev(dev);
}

int UsbIO::open(int)
{
    dev = NULL;
//    usb_getDeviceWithSerial(USB_VID ,USB_PID ,serial ,devHandler ,this);
    usb_getDeviceWithAddress(USB_VID ,USB_PID ,address ,devHandler ,this);
    if(!dev){
        return -2;
    }
    struct_deviceInterface di;
    di.interface = -1;
    if(!usb_open(dev ,&di)){
        intf = di.intf;
        inPipeRef = di.inPipeRef;
        outPipeRef = di.outPipeRef;
        interface = di.interface;
        LOG_PARA("in pipe:%d ,out pipe:%d" ,inPipeRef ,outPipeRef);
        return 0;
    }
    return -1;
}

int UsbIO::close(void)
{
    inPipeRef = 0;
    outPipeRef = 0;
    int ret = usb_close(dev ,intf);
    dev = NULL;
    intf = NULL;
    return ret;
}

int UsbIO::write(char *buffer, int bufsize)
{
    return usb_writePipe(intf ,outPipeRef ,buffer ,bufsize);
}

int UsbIO::read(char *buffer, int bufsize)
{
//    return usb_readPipeAsync(intf ,inPipeRef ,buffer ,bufsize);
    return usb_readPipe(intf ,inPipeRef ,buffer ,bufsize);
}

int UsbIO::writeCommand(char *buffer, int bufsize)
{
    return write(buffer ,bufsize);
//    return usb_dev_write(dev ,interface ,buffer ,bufsize);
//    return usb_intf_write(intf ,interface ,buffer ,bufsize);
}

int UsbIO::readCommand(char *buffer, int bufsize)
{
    return read(buffer ,bufsize);
//    return usb_dev_read(dev ,interface ,buffer ,bufsize);
//    return usb_intf_read(intf ,interface ,buffer ,bufsize);
}

int UsbIO::resolveUrl(const char* url)
{
//    bool ok;
//    address = QUrlQuery(QUrl(url)).queryItemValue("address").toInt(&ok);
//    if(!ok){
//        return -1;
//    }
    strcpy(serial ,QUrlQuery(QUrl(url)).queryItemValue("address").toLatin1().data());
    usb_getDeviceWithSerial(USB_VID ,USB_PID ,serial ,devHandler ,this);
    if(!dev){
        address = -1;
        return -1;
    }
    address = usb_getAddress(dev);
    return 0;
}

bool UsbIO::isConnected()
{
    bool connected = true;
    dev = NULL;
    usb_getDeviceWithAddress(USB_VID ,USB_PID ,address ,devHandler ,this);
//    usb_getDeviceWithSerial(USB_VID ,USB_PID ,serial ,devHandler ,this);
    if(!dev){
        connected = false;
    }
    return connected;
}
