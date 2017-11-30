#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOCFPlugIn.h>
#include "mac_usb.h"
#include "../log.h"
#define VERBOSE 1
static Boolean foundOnce  = false;


static void printInterpretedError(const char *s, IOReturn err)
{

UInt32 system, sub, code;

    LOG_PARA( "%s (0x%08X) ", s, err);

    system = err_get_system(err);
    sub = err_get_sub(err);
    code = err_get_code(err);

    if(system == err_get_system(sys_iokit))
    {
        if(sub == err_get_sub(sub_iokit_usb))
        {
            LOG_PARA( "USB error %d(0x%X) ", (int)code, (unsigned int)code);
        }
        else if(sub == err_get_sub(sub_iokit_common))
        {
            LOG_PARA( "IOKit common error %d(0x%X) ", (int)code, (unsigned int)code);
        }
        else
        {
            LOG_PARA( "IOKit error %d(0x%X) from subsytem %d(0x%X) ", (int)code, (unsigned int)code, (int)sub, (unsigned int)sub);
        }
    }
    else
    {
        LOG_PARA( "error %d(0x%X) from system %d(0x%X) - subsytem %d(0x%X) ", (int)code, (unsigned int)code, (int)system, (unsigned int)system, (int)sub, (unsigned int)sub);
    }
}

static bool get_serial(IOUSBDeviceInterface_version  **dev ,char* serial)
{
    kern_return_t            kr;
//    UInt16                   vendor;
//    UInt16                   product;
    UInt8                    serialIndex;

//    kr = (*dev)->GetDeviceVendor(dev, &vendor);
//    kr = (*dev)->GetDeviceProduct(dev, &product);
    kr = (*dev)->USBGetSerialNumberStringIndex(dev, &serialIndex);

    if (serialIndex > 0) {
        IOUSBDevRequest req;
        UInt16          buffer[256];
        UInt16          languages[128];

        memset(languages, 0, sizeof(languages));

        req.bmRequestType =
            USBmakebmRequestType(kUSBIn, kUSBStandard, kUSBDevice);
        req.bRequest = kUSBRqGetDescriptor;
        req.wValue = (kUSBStringDesc << 8) | 0;
        req.wIndex = 0;
        req.pData = languages;
        req.wLength = sizeof(languages);
        kr = (*dev)->DeviceRequest(dev, &req);

        if (kr == kIOReturnSuccess && req.wLenDone > 0) {

            int langCount = (req.wLenDone - 2) / 2, lang;

            for (lang = 1; lang <= langCount; lang++) {

                memset(buffer, 0, sizeof(buffer));
                memset(&req, 0, sizeof(req));

                req.bmRequestType =
                    USBmakebmRequestType(kUSBIn, kUSBStandard, kUSBDevice);
                req.bRequest = kUSBRqGetDescriptor;
                req.wValue = (kUSBStringDesc << 8) | serialIndex;
                req.wIndex = languages[lang];
                req.pData = buffer;
                req.wLength = sizeof(buffer);
                kr = (*dev)->DeviceRequest(dev, &req);

                if (kr == kIOReturnSuccess && req.wLenDone > 0) {
                    int i, count;

                    // skip first word, and copy the rest to the serial string,
                    // changing shorts to bytes.
                    count = (req.wLenDone - 1) / 2;
                    for (i = 0; i < count; i++)
                        serial[i] = buffer[i + 1];
                    serial[i] = 0;
                    return true;
                }
            }
        }
    }
    return false;
}


void useUSBInterface(IOUSBInterfaceInterface_version **intf  ,struct_deviceInterface* pDeviceInterface)
{
//    LOG_NOPARA("Now we actually get to do something with this device, wow!!!!\n");
//    finallyDoSomethingWithThisDevice(intf);
    pDeviceInterface->intf = intf;
}

static UInt32 openUSBInterface(IOUSBInterfaceInterface_version **intf ,struct_deviceInterface* pDeviceInterface)
{
    IOReturn ret;

//    #if VERBOSE
    UInt8 n;
    int i;
    UInt8 direction;
    UInt8 number;
    UInt8 transferType;
    UInt16 maxPacketSize;
    UInt8 interval;
//    static char *types[]={
//            "Control",
//            "Isochronous",
//            "Bulk",
//            "Interrupt"};
//    static char *directionStr[]={
//            "Out",
//            "In",
//            "Control"};
//    #endif

        ret = (*intf)->USBInterfaceOpen(intf);
        if(ret != kIOReturnSuccess)
        {
            printInterpretedError("Could not set configuration on device", ret);
            return(-1);
        }

//    #if VERBOSE
        // We don't use the endpoints in our device, but it has some anyway

        ret = (*intf)->GetNumEndpoints(intf, &n);
        if(ret != kIOReturnSuccess)
        {
            printInterpretedError("Could not get number of endpoints in interface", ret);
            return(0);
        }

//        LOG_PARA("%d endpoints found\n", n);

        for(i = 1; i<=n; i++)
        {
            ret = (*intf)->GetPipeProperties(intf, i, &direction, &number, &transferType, &maxPacketSize, &interval);
            if(ret != kIOReturnSuccess)
            {
                LOG_PARA( "Endpoint %d -", n);
                printInterpretedError("Could not get endpoint properties", ret);
                return(0);
            }
//            LOG_PARA("Endpoint %d: %s %s %d, max packet %d, interval %d\n", i, types[transferType], directionStr[direction], number, maxPacketSize, interval);

            if (transferType == kUSBBulk && direction == kUSBIn)
            {
                // grabbing BULK IN pipe index i
                pDeviceInterface->inPipeRef = i;
//                maxInPacketSize = maxPacketSize;
                continue;
            }

            if (transferType == kUSBBulk && direction == kUSBOut)
            {
                // grabbing BULK OUT pipe index i
                pDeviceInterface->outPipeRef = i;
//                maxOutPacketSize = maxPacketSize;
                continue;
            }
        }


//    #endif
        return(0);
}

static IOUSBInterfaceInterface_version **getUSBInterfaceInterface(io_service_t usbInterface)
{
    IOReturn err;
    IOCFPlugInInterface **plugInInterface=NULL;
    IOUSBInterfaceInterface_version **intf=NULL;
    SInt32 score;
    HRESULT res;

    // There is no documentation for IOCreatePlugInInterfaceForService or QueryInterface, you have to use sample code.

    err = IOCreatePlugInInterfaceForService(usbInterface,
                kIOUSBInterfaceUserClientTypeID,
                kIOCFPlugInInterfaceID,
                &plugInInterface,
                &score);
    (void)IOObjectRelease(usbInterface);				// done with the usbInterface object now that I have the plugin
    if ((kIOReturnSuccess != err) || (plugInInterface == nil) )
    {
        printInterpretedError("Unable to create plug in interface for USB interface", err);
        return(nil);
    }

    res = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID_version), (LPVOID*)(&intf));
    IODestroyPlugInInterface(plugInInterface);			// done with this

    if (res || !intf)
    {
        LOG_PARA( "Unable to create interface with QueryInterface %X\n", (unsigned int)res);
        return(nil);
    }
    return(intf);
}

Boolean isThisTheInterfaceYoureLookingFor(IOUSBInterfaceInterface_version **intf  ,struct_deviceInterface* pDeviceInterface)
{
    //	Check to see if this is the interface you're interested in
    //  This code is only expecting one interface, so returns true
    //  the first time.
    //  You code could check the nature and type of endpoints etc

    if(foundOnce)
    {
        LOG_PARA( "Subsequent interface found, we're only intersted in 1 of them\n");
        return(false);
    }
    UInt8 interface;
    (*intf)->GetInterfaceNumber(intf ,&interface);
    LOG_PARA("usb interface is:%d" ,interface);
    if(pDeviceInterface->interface < 0){
        foundOnce = true;
    }else{
        if(interface == pDeviceInterface->interface){
            foundOnce = true;
        }
    }
    return(foundOnce);
}

static int iterateinterfaces(io_iterator_t interfaceIterator  ,struct_deviceInterface* pDeviceInterface)
{
    io_service_t usbInterface;
    int err = 0;
    //IOReturn ret;
    IOUSBInterfaceInterface_version **intf=NULL;

    usbInterface = IOIteratorNext(interfaceIterator);
    if(usbInterface == IO_OBJECT_NULL)
    {
        LOG_PARA( "Unable to find an Interface\n");
        return(-1);
    }

    foundOnce = false;
    while(usbInterface != IO_OBJECT_NULL)
    {
        intf = getUSBInterfaceInterface(usbInterface);

        if(intf != nil)
        {
  // Don't release the interface here. That's one too many releases and causes set alt interface to fail
            if(isThisTheInterfaceYoureLookingFor(intf ,pDeviceInterface))
            {
                err = openUSBInterface(intf ,pDeviceInterface);
                if(err == 0)
                {
                    useUSBInterface(intf ,pDeviceInterface);
//                    ret = (*intf)->USBInterfaceClose(intf);
                }

//                ret = (*intf)->Release(intf);
                // Not worth bothering with errors here
                return(err);
            }
        }
        usbInterface = IOIteratorNext(interfaceIterator);
    }

    LOG_PARA( "No interesting interfaces found\n");
    IOObjectRelease(usbInterface);
    return(-1);
}



static IOReturn useUSBDevice(IOUSBDeviceInterface_version **dev, UInt32 configuration ,struct_deviceInterface* pDeviceInterface)
{
    io_iterator_t interfaceIterator;
    IOUSBFindInterfaceRequest req;
    IOReturn err;

    err = (*dev)->SetConfiguration(dev, configuration);
    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not set configuration on device", err);
        return err;
    }

    req.bInterfaceClass = kIOUSBFindInterfaceDontCare;
    req.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
    req.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
    req.bAlternateSetting = kIOUSBFindInterfaceDontCare;

    err = (*dev)->CreateInterfaceIterator(dev, &req, &interfaceIterator);
    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not create interface iterator", err);
        return err;
    }


    err = iterateinterfaces(interfaceIterator ,pDeviceInterface);

    IOObjectRelease(interfaceIterator);

    return err;
}

static SInt32 openUSBDevice(IOUSBDeviceInterface_version **dev)
{
    UInt8 numConfig;
    IOReturn err;
    IOUSBConfigurationDescriptorPtr desc;

    err = (*dev)->GetNumberOfConfigurations(dev, &numConfig);
    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not number of configurations from device", err);
        return(-1);
    }
    if(numConfig != 1)
    {
        LOG_PARA( "This does not look like the right device, it has %d configurations (we want 1)\n", numConfig);
        return(-1);
    }

    err = (*dev)->GetConfigurationDescriptorPtr(dev, 0, &desc);
    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not get configuration descriptor from device", err);
        return(-1);
    }

#if VERBOSE
    LOG_PARA("Configuration value is %d\n", desc->bConfigurationValue);
#endif

    // We should really try to do classic arbitration here

    err = (*dev)->USBDeviceOpen(dev);
    if(err == kIOReturnExclusiveAccess)
    {
#if VERBOSE
        LOG_NOPARA("Exclusive error opening device, we may come back to this later\n");
#endif
        return(-2);
    }
    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not open device", err);
        return(-1);
    }


    return(desc->bConfigurationValue);
}

static IOUSBDeviceInterface_version **getUSBDevice(io_object_t usbDevice)
{
    IOReturn err;
    IOCFPlugInInterface **plugInInterface=NULL;
    IOUSBDeviceInterface_version **dev=NULL;
    SInt32 score;
    HRESULT res;

    // There is no documentation for IOCreatePlugInInterfaceForService or QueryInterface, you have to use sample code.

    err = IOCreatePlugInInterfaceForService(usbDevice,
                kIOUSBDeviceUserClientTypeID,
                kIOCFPlugInInterfaceID,
                &plugInInterface,
                &score);
    if ((kIOReturnSuccess != err) || (plugInInterface == nil) )
    {
        printInterpretedError("Unable to create plug in interface for USB device", err);
        return(nil);
    }

    res = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID_version), (LPVOID*)&dev);
    IODestroyPlugInInterface(plugInInterface);			// done with this

    if (res || !dev)
    {
        LOG_PARA( "Unable to create USB device with QueryInterface\n");
        return(nil);
    }

#if VERBOSE
    {
    UInt16 VID, PID, REL;
        err = (*dev)->GetDeviceVendor(dev, &VID);
        err = (*dev)->GetDeviceProduct(dev, &PID);
        err = (*dev)->GetDeviceReleaseNumber(dev, &REL);

        LOG_PARA("Found device VID 0x%04X (%d), PID 0x%04X (%d), release %d\n", VID, VID, PID, PID, REL);
    }
#endif

    return(dev);
}


int iterateDevices(CFMutableDictionaryRef dict ,usbDeviceHandler handler ,void* handlerData)
{
    IOReturn err;
    mach_port_t masterPort;

    io_iterator_t anIterator;

    err = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not get master port", err);
        return -1;//
    }

    err = IOServiceGetMatchingServices(masterPort, dict, &anIterator);

    if(err != kIOReturnSuccess)
    {
        printInterpretedError("Could not get device iterator", err);
        return -1;//
    }

    io_object_t usbDevice;
    IOUSBDeviceInterface_version **dev=NULL;
    usbDevice = IOIteratorNext(anIterator);
    if(usbDevice == IO_OBJECT_NULL)
    {
        LOG_PARA( "Unable to find first matching USB device\n");
        return -2;
    }

    int ret;
    while(usbDevice != IO_OBJECT_NULL)
    {
        dev = getUSBDevice(usbDevice);
        IOObjectRelease(usbDevice);
        if(handler){
            ret = handler(dev ,handlerData);
            if(ret <= 0)
                return ret;
        }else{
            return -1;
        }
        usbDevice = IOIteratorNext(anIterator);
    };

    IOObjectRelease(anIterator);
    return 0;
}

static int _getUsbDeviceList(IOUSBDeviceInterface_version** dev ,void* pData)
{
    if(!pData || !dev)
        return -1;
    struct_deviceList* pData_deviceList = (struct_deviceList*)pData;
    struct_deviceInfo* pDevice = pData_deviceList->deviceList + pData_deviceList->length;
    (*dev)->GetLocationID(dev ,(UInt32*)&pDevice->locationID);
    memset(pDevice->serial ,0 ,sizeof(pDevice->serial));
    get_serial(dev ,pDevice->serial);
    USBDeviceAddress uda;
    (*dev)->GetDeviceAddress(dev ,&uda);
    pDevice->address = (int)uda;
    pData_deviceList->length += 1;
    if(pData_deviceList->length >= pData_deviceList->maxLength)
        return 0;
    return 1;
}

static CFMutableDictionaryRef getUsbDeviceDict(int vid ,int pid)
{

    CFMutableDictionaryRef dict;
    dict = IOServiceMatching("IOUSBDevice");

    if(dict == nil)
    {
        LOG_NOPARA( "Could create matching dictionary\n");
        return nil;
    }
    CFDictionarySetValue(
                dict,
                CFSTR(kUSBVendorID),
                CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &vid)
                );
    CFDictionarySetValue(dict, CFSTR(kUSBProductID),
                CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &pid));
    return dict;
}

int usb_getDeviceList(int vid ,int pid ,addUsbDevice addDevice ,void* pData)
{
    if(!addDevice)
        return -2;
    CFMutableDictionaryRef dict = getUsbDeviceDict(vid ,pid);
    if(dict == nil)
        return -1;

    struct_deviceInfo deviceList[10];
    struct_deviceList dl;
    dl.deviceList = deviceList;
    dl.length = 0;
    dl.maxLength = sizeof(deviceList) / sizeof(struct_device);
    int ret = iterateDevices(dict ,_getUsbDeviceList ,&dl);
    if(!ret){
        for(int i = 0; i < dl.length ;i++){
            addDevice(dl.deviceList + i ,pData);
        }
    }
    return ret;
}

static int _getUsbDeviceWithLocationID(IOUSBDeviceInterface_version** dev ,void* pData)
{
    if(!pData || !dev)
        return -1;
    int ret;
    int locationID = 0;
    (*dev)->GetLocationID(dev ,(UInt32*)&locationID);
    struct_device* pData_device = (struct_device*)pData;
    if(locationID == pData_device->deviceInfo.locationID){
        pData_device->dev = dev;
        ret = 0;
    }else{
        pData_device->dev = NULL;
        ret = 1;
    }
    return ret;
}

int usb_getDeviceWithLocationID(int vid ,int pid ,int locationID ,usbDeviceHandler handler ,void* pData)
{
    CFMutableDictionaryRef dict = getUsbDeviceDict(vid ,pid);
    if(dict == nil)
        return -1;

    struct_device data;
    data.deviceInfo.locationID = locationID;
    int ret = iterateDevices(dict ,_getUsbDeviceWithLocationID ,&data);
    if(!ret){
        if(handler)
            handler(data.dev ,pData);
    }
    return ret;
}

static int _getUsbDeviceWithSerail(IOUSBDeviceInterface_version** dev ,void* pData)
{
    if(!pData || !dev)
        return -1;
    int ret;
    char device_serial[256];
    memset(device_serial ,0 ,sizeof(device_serial));
    get_serial(dev ,device_serial);
    struct_device* pData_device = (struct_device*)pData;
    if(!strcmp(device_serial ,pData_device->deviceInfo.serial)){
        pData_device->dev = dev;
        ret = 0;
    }else{
        pData_device->dev = NULL;
        ret = 1;
    }
    return ret;
}

int usb_getDeviceWithSerial(int vid ,int pid ,const char* serial ,usbDeviceHandler handler ,void* pData)
{
    CFMutableDictionaryRef dict = getUsbDeviceDict(vid ,pid);
    if(dict == nil)
        return -1;

    struct_device data;
    memset((void*)&data ,0 ,sizeof(data));
    strcpy(data.deviceInfo.serial ,serial);
    int ret = iterateDevices(dict ,_getUsbDeviceWithSerail ,&data);
    if(!ret){
        if(handler)
            handler(data.dev ,pData);
    }
    return ret;
}


static int _getUsbDeviceWithAddress(IOUSBDeviceInterface_version** dev ,void* pData)
{
    if(!pData || !dev)
        return -1;
    int ret;
    USBDeviceAddress address = -1;
    (*dev)->GetDeviceAddress(dev ,&address);
    struct_device* pData_device = (struct_device*)pData;
    if((int)address == pData_device->deviceInfo.address){
        pData_device->dev = dev;
        ret = 0;
    }else{
        pData_device->dev = NULL;
        ret = 1;
    }
    return ret;
}

int usb_getDeviceWithAddress(int vid ,int pid ,int address ,usbDeviceHandler handler ,void* pData)
{
    CFMutableDictionaryRef dict = getUsbDeviceDict(vid ,pid);
    if(dict == nil)
        return -1;

    struct_device data;
    data.deviceInfo.address = address;
    int ret = iterateDevices(dict ,_getUsbDeviceWithAddress ,&data);
    if(!ret){
        if(handler)
            handler(data.dev ,pData);
    }
    return ret;
}

bool usb_isConnected(IOUSBDeviceInterface_version** dev)
{
    if(!dev)
        return false;
    IOReturn ior;
    USBDeviceAddress address = -1;
    ior = (*dev)->GetDeviceAddress(dev ,&address);
    return (ior == kIOReturnSuccess);
}

int usb_open(IOUSBDeviceInterface_version **dev ,struct_deviceInterface* pDeviceInterface)
{
    if(!dev)
        return -2;
    SInt32 config = 0;
    int exclusiveErrs, attempts;

    for(attempts = 1; attempts < 5; attempts++)
    {
        exclusiveErrs = 0;
        config = openUSBDevice(dev);
        if(config == -2){
            exclusiveErrs++;
        }
        else if(config >= 0){
            // Device sucessfully opened

            if(config > 0){
                return useUSBDevice(dev, config ,pDeviceInterface);
            }else{
                LOG_NOPARA("What use is a device with a zero configuration????\n");
                (*dev)->USBDeviceClose(dev);
            }

        }

        if(exclusiveErrs > 0){
            sleep(1);
            LOG_PARA("Trying open again %d\n", attempts);
        }else{
            break;
        }
    }
    return(-1);
}

int usb_close(IOUSBDeviceInterface_version **dev ,IOUSBInterfaceInterface_version **intf)
{
    if(intf){
        (*intf)->USBInterfaceClose(intf);
        (*intf)->Release(intf);
    }
    if(dev){
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
    }
    return 0;
}

int usb_writePipe(IOUSBInterfaceInterface_version **intf ,int outPipeRef ,char *buffer, size_t bufsize)
{

    if(!intf || !outPipeRef)
        return (-2);//
    IOReturn err;
    err = (*intf)->WritePipeTO(intf , outPipeRef, buffer, bufsize,5000,5000);

    switch (err) {
    case kIOReturnNoDevice:
        LOG_NOPARA("kIOReturnNoDevice if there is no connection to an IOService");
        break;
    case kIOReturnAborted:
        LOG_NOPARA("kIOReturnAborted if the thread is interrupted before the call completes");
        break;
    case kIOReturnNotOpen:
        LOG_NOPARA("kIOReturnNotOpen if the interface is not open for exclusive access");
        break;
    case kIOReturnSuccess:
        return bufsize;
    default:
        break;
    }
    return (-1);
}

int usb_readPipe(IOUSBInterfaceInterface_version **intf ,int inPipeRef ,char *buffer, size_t bufsize)
{
    if(!intf || !inPipeRef)
        return (-2);//
    IOReturn err;
    UInt32 size = bufsize;
    err = (*intf)->ReadPipeTO(intf , inPipeRef, buffer, &size,10000,5000);

    switch (err) {
    case kIOReturnNoDevice:
        LOG_NOPARA("kIOReturnNoDevice if there is no connection to an IOService");
        break;
    case kIOReturnAborted:
        LOG_NOPARA("kIOReturnAborted if the thread is interrupted before the call completes");
        break;
    case kIOReturnNotOpen:
        LOG_NOPARA("kIOReturnNotOpen if the interface is not open for exclusive access");
        break;
    case kIOReturnBadArgument:
        LOG_NOPARA("timeout values are specified for an interrupt pipe.");
        break;
    case kIOReturnSuccess:
//        return bufsize;
        return size;
    default:
        break;
    }
    return (-1);
}
