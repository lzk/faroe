#ifndef DEVICE_H
#define DEVICE_H

#define SETTING_IMG_1_BIT	1
#define SETTING_IMG_8_BIT	8
#define SETTING_IMG_24_BIT	24
enum{
    SETTING_DOC_SIZE_FULL,
    SETTING_DOC_SIZE_A4,
    SETTING_DOC_SIZE_LT
};
enum{
    SETTING_SCAN_FLB,
    SETTING_SCAN_ADF
};
enum{
    SETTING_IMG_FORMAT_JPG,
    SETTING_IMG_FORMAT_RAW
};
#define SETTING_SCAN_A_SIDE		1
#define SETTING_SCAN_B_SIDE		2
#define SETTING_SCAN_AB_SIDE	3
#define SETTING_SCAN_DUPLEX   SETTING_SCAN_AB_SIDE   // 1: A_side, 2: B_side, 3: AB_sides
#define SETTING_SCAN_OPTION   0
#define SETTING_SCAN_PAGE     0   // 0: adf sensor detect

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

class Scanner;
class DeviceIO;
class Setter;
class PlatformApp;
class Device
{
public:
    Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp);
    virtual ~Device();
    int scan(void* setting);
    bool checkConnection();
    int deviceCmd(int cmd ,void* data);

    Scanner* getScanner();

    static void searchDevices(addDeviceHandler handler,void* pData);
private:
    DeviceIO* deviceIO;
    Scanner* scanner;
    Setter* setter;

//    int open(const char* );
    int open();
    int close();
};

}
#endif // DEVICE_H
