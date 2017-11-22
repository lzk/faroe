#include "usbio.h"
#include "mac_usb.h"
#include "../../lld/device.h"
#include <string.h>
#include <QUrlQuery>
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
    strcpy(di.name ,"usbscan");
    sprintf(di.address ,"%02d" ,pDeviceInfo->address);
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

int UsbIO::open()
{
    if(!dev){
        return -2;
    }
    struct_deviceInterface di;
    di.interface = -1;
    if(!usb_open(dev ,&di)){
        intf = di.intf;
        inPipeRef = di.inPipeRef;
        outPipeRef = di.outPipeRef;
        return 0;
    }
    return -1;
}

int UsbIO::close(void)
{
    inPipeRef = 0;
    outPipeRef = 0;
    return usb_close(dev ,intf);
}

int UsbIO::write(char *buffer, int bufsize)
{
    return usb_writePipe(intf ,outPipeRef ,buffer ,bufsize);
}

int UsbIO::read(char *buffer, int bufsize)
{
    return usb_readPipe(intf ,inPipeRef ,buffer ,bufsize);
}

int UsbIO::resolveUrl(const char* url)
{
    bool ok;
    int address = QUrlQuery(url).queryItemValue("address").toInt(&ok);
    if(!ok){
        return -1;
    }
    usb_getDeviceWithAddress(USB_VID ,USB_PID ,address ,devHandler ,this);
    if(!dev){
        return -2;
    }
    return 0;
}

bool UsbIO::isConnected()
{
    return usb_isConnected(dev);
}
