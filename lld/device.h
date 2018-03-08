#ifndef DEVICE_H
#define DEVICE_H

#define SETTING_IMG_1_BIT	1
#define SETTING_IMG_8_BIT	8
#define SETTING_IMG_24_BIT	24
enum{
    SETTING_papersize_auto,
    SETTING_papersize_autoNoMultiFeed,
    SETTING_papersize_A4,
    SETTING_papersize_A5,
    SETTING_papersize_B5,
    SETTING_papersize_A6,
    SETTING_papersize_letter,
    SETTING_papersize_legal,
    SETTING_papersize_long,
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
    enum {
        CMD_PRINT,
        CMD_SCAN,
        CMD_setWifi,
        CMD_getWifiInfo,
        CMD_setPassword,
        CMD_confirmPassword,
        CMD_setSaveTime,
        CMD_getSaveTime,
        CMD_setOffTime,
        CMD_getOffTime,
        CMD_getIpv4,
        CMD_setIpv4,
        CMD_setSoftap,
        CMD_getSoftap,
        CMD_getDeviceSetting,
        CMD_setDeviceSetting,
        CMD_getPowerSupply,
    };
public:
    Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp);
    virtual ~Device();
    void resolveUrl(const char* url);
    bool checkConnection();
    int deviceCmd(int cmd ,void* data);
    const char* getCurrentUrl(){return currentUrl;}

    Scanner* getScanner();

    static void searchUsbDevices(addDeviceHandler handler,void* pData);
    static void searchDevices(addDeviceHandler handler,void* pData);
    static void cancelSearch();
private:   
    DeviceIO* deviceIO;
    Scanner* scanner;
    Setter* setter;
    char currentUrl[256];
};

}
#endif // DEVICE_H
