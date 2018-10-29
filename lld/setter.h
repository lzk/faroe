#ifndef SETTER_H
#define SETTER_H
namespace JK {

class DeviceIO;
class PlatformApp;
class SetterApi;
class Setter
{
public:
    typedef struct{
        bool enable;
        int type;
        int encryption;
        int wepKeyId;
        int channel;
        char ssid[32];
        char password[64];
    }
        struct_wifiSetting;

    typedef struct
    {
        char ssid[33];
        unsigned char encryption;
    }
        struct_aplist;

    typedef struct{
        struct_aplist aplist[10];
        struct_wifiSetting wifiSetting;
    }
        struct_wifiInfo;

    typedef struct
    {
        char ssid[33];
        unsigned char connected;
    }
        struct_softaplist;

    typedef struct{
        bool enable;
        bool wifiEnable;
        char ssid[32];
        char password[64];
        struct_softaplist aplist[10];
    }
        struct_softAp;
    typedef struct{
        int mode;
        int addressMode;
        bool wifiEnable;
        unsigned char address[4];
        unsigned char subnetMask[4];
        unsigned char gatewayAddress[4];
    }
        struct_ipv4;

    typedef struct{
        int saveTime;
        int offTime;
    }
        struct_deviceSetting;

public:
    Setter();
    ~Setter();
    void install(DeviceIO* dio ,PlatformApp* platformApp);

    int getDeviceStatus(void*);
    int setWifi(void*);
    int getWifi(void*);
    int confirmPassword(void* password);
    int setPassword(void* password);
    int setSoftAp(void* softap);
    int getSoftAp(void* softap);
    int setIpV4(void* ip);
    int getIpV4(void* ip);
    int setSaveTime(void* st);
    int getSaveTime(void* st);
    int setOffTime(void* st);
    int getOffTime(void* st);
    int setDeviceSetting(void* ds);
    int getDeviceSetting(void* ds);

private:
    DeviceIO* dio;
    SetterApi* setterApi;
    bool isLogined;
};

}
#endif // SETTER_H
