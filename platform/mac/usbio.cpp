#include "usbio.h"
#include "mac_usb.h"
#include "../../lld/device.h"
#include <string.h>
#include <QUrlQuery>
#include "../log.h"

typedef struct struct_product{
    int vid;
    int pid;
}
    struct_product;

static const struct_product products_list[] = {
    {0x05e3 ,0x0118},//faroe
    {0x2e5a ,0xf130},//Q250
    {0x2e5a ,0xf133},//Q2130
    {0x2e5a ,0xf137},//Q2135
    {0x2e5a ,0xf13a},//all A
    {0x2e5a ,0xf13b},//all B
    {0x2e5a ,0xf13c},//all C
};
static int num_of_products = sizeof(products_list) / sizeof(products_list[0]);

typedef struct struct_searchData{
    addDeviceHandler handler;
    void* pData;
    int product_index;
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
    di.vid = products_list[psd->product_index].vid;
    di.pid = products_list[psd->product_index].pid;
    if(psd->handler)
        psd->handler(&di ,psd->pData);
}

void usbSearchDevices(addDeviceHandler handler,void* pData)
{
    struct_searchData sd;
    sd.handler = handler;
    sd.pData = pData;
    for(int i = 0 ;i < num_of_products ;i++){
        LOG_PARA("searching device VID 0x%04X (%d), PID 0x%04X (%d)", products_list[i].vid, products_list[i].vid, products_list[i].pid, products_list[i].pid);
        sd.product_index = i;
        usb_getDeviceList(products_list[i].vid ,products_list[i].pid ,addDeviceInfo ,&sd);
    }
}

UsbIO::UsbIO() :
    inPipeRef(0)
  ,outPipeRef(0)
  ,address(-1)
  ,vid(-1)
  ,pid(-1)
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
    usb_getDeviceWithAddress(vid ,pid ,address ,devHandler ,this);
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
    strcpy(serial ,QUrlQuery(QUrl(url)).queryItemValue("address").toUtf8().data());

    vid = -1;
    pid = -1;
    address = -1;
    for(int i = 0 ;i < num_of_products ;i++){
        usb_getDeviceWithSerial(products_list[i].vid ,products_list[i].pid ,serial ,devHandler ,this);
        if(dev){
            address = usb_getAddress(dev);
            vid = products_list[i].vid;
            pid = products_list[i].pid;
            LOG_PARA("Found device VID 0x%04X (%d), PID 0x%04X (%d), release %s", vid, vid, pid, pid, serial);
            return 0;
        }
    }
    return -1;
}

bool UsbIO::isConnected()
{
    if(vid == -1 || pid == -1 || address == -1)
        return false;
    bool connected = true;
    dev = NULL;
    usb_getDeviceWithAddress(vid ,pid ,address ,devHandler ,this);
//    usb_getDeviceWithSerial(USB_VID ,USB_PID ,serial ,devHandler ,this);
    if(!dev){
        connected = false;
    }
    return connected;
}
