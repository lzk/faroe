#include "setter.h"
#include "setterapi.h"
#include "deviceio.h"
//#include "device.h"
#include "type.h"
#include "setterstruct.h"
#include "../platform/log.h"
#include <string.h>
#include <stdlib.h>
#include "../platform/devicestruct.h"
using namespace JK;
static int _base64_char_value(char base64char)
 {
    if (base64char >= 'A' && base64char <= 'Z')
         return base64char-'A';
    if (base64char >= 'a' && base64char <= 'z')
         return base64char-'a'+26;
    if (base64char >= '0' && base64char <= '9')
         return base64char-'0'+2*26;
    if (base64char == '+')
         return 2*26+10;
    if (base64char == '/')
         return 2*26+11;
    return -1;
}

static int _base64_decode_triple(char quadruple[4], unsigned char *result)
 {
    int i, triple_value, bytes_to_decode = 3, only_equals_yet = 1;
    int char_value[4];

    for (i=0; i<4; i++)
         char_value[i] = _base64_char_value(quadruple[i]);

    for (i=3; i>=0; i--)
    {
         if (char_value[i]<0)
         {
             if (only_equals_yet && quadruple[i]=='=')
             {
                  char_value[i]=0;
                  bytes_to_decode--;
                  continue;
             }
             return 0;
         }
         only_equals_yet = 0;
    }

    if (bytes_to_decode < 0)
         bytes_to_decode = 0;

    triple_value = char_value[0];
    triple_value *= 64;
    triple_value += char_value[1];
    triple_value *= 64;
    triple_value += char_value[2];
    triple_value *= 64;
    triple_value += char_value[3];

    for (i=bytes_to_decode; i<3; i++)
         triple_value /= 256;
    for (i=bytes_to_decode-1; i>=0; i--)
    {
         result[i] = triple_value%256;
         triple_value /= 256;
    }

    return bytes_to_decode;
}

static size_t Base64Decode(char *source, unsigned char *target, size_t targetlen)
 {
    char *src, *tmpptr;
    char quadruple[4], tmpresult[3];
    int i; size_t tmplen = 3;
    size_t converted = 0;

    src = (char *)malloc(strlen(source)+5);
    if (src == NULL)
         return -1;
    strcpy(src, source);
    strcat(src, "====");
    tmpptr = src;
    while (tmplen == 3)
    {
         /* get 4 characters to convert */
         for (i=0; i<4; i++)
         {
             while (*tmpptr != '=' && _base64_char_value(*tmpptr)<0)
                  tmpptr++;
             quadruple[i] = *(tmpptr++);
         }
         tmplen = _base64_decode_triple(quadruple, (unsigned char*)tmpresult);
         if (targetlen < tmplen)
         {
             free(src);
             return converted;
         }
         memcpy(target, tmpresult, tmplen);
         target += tmplen;
         targetlen -= tmplen;
         converted += tmplen;
    }
    free(src);
    return converted;
}

int getDESfromDeviceID(char* device_id ,char* str)
{
    if (device_id==NULL) {
        return -1;
    }
    char *p = device_id;

    while (*p && strncmp(p,"DES:",4)!=0) // Look for "DES:"
        p++;

    if (!*p)	{ // "DES:" not found
        LOG_NOPARA("DES: not found");
        return -1;
    }
    p += 4;	// Skip "DES:"

    char* q = p;
    while(*q && ';'!= *q)
        q++;
    if(!*q){
        LOG_NOPARA("DES:no \';\'");
        return -1;
    }

    memcpy(str ,p ,q-p);
    str[q - p] = 0;
    return 0;
}

Setter::Setter():
    setterApi(new SetterApi)
  ,isLogined(false)
{

}

Setter::~Setter()
{
    delete setterApi;
}

void Setter::install(DeviceIO* dio ,PlatformApp* platformApp)
{
    this->dio = dio;
    setterApi->install(dio);
//    pPlatformApp = platformApp;
}

int Setter::getDeviceStatus(void*)
{
    return 0;
}

int Setter::confirmPassword(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    const char* password =(const char*)data;
    int err;
    if(!isLogined){
        cmdst_passwd para;
        memset(&para ,0 ,sizeof(para));
        memcpy(para.passwd ,password ,32);
        err = setterApi->cmd(CMD_CODE_confirmPasswd ,&para ,sizeof(para));
        if(!err){
            isLogined = true;
        }
    }
}

int Setter::setPassword(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    const char* password =(const char*)data;
    int err;
    if(!isLogined)
        return DeviceStruct::ERR_no_logined;
    cmdst_passwd para;
    memset(&para ,0 ,sizeof(para));
    memcpy(para.passwd ,password ,32);
    err = setterApi->cmd(CMD_CODE_setPasswd ,&para ,sizeof(para));
}

int Setter::setWifi(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    struct_wifiSetting* wifiSetting = (struct_wifiSetting*)data;
    int err;
    if(!isLogined)
        return DeviceStruct::ERR_no_logined;

    cmdst_wifi_get para;
    memset(&para ,0 ,sizeof(para));
    if(wifiSetting->enable){
        para.wifiEnable = 1;
        para.channel = wifiSetting->channel;
        para.encryption = wifiSetting->encryption;
        para.netType = wifiSetting->type;
        para.wepKeyId = wifiSetting->wepKeyId;
        memcpy(para.ssid ,wifiSetting->ssid ,32);
        memcpy(para.pwd ,wifiSetting->password ,64);

    }else{
        para.wifiEnable = 0;
    }

    if(DeviceIO::Type_net == dio->type())
        err = setterApi->cmd(CMD_CODE_setWifiInfo_noRead ,&para ,sizeof(para));
    else
        err = setterApi->cmd(CMD_CODE_setWifiInfo ,&para ,sizeof(para));

    return err;
}

int Setter::getWifi(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    struct_wifiInfo* wifiInfo = (struct_wifiInfo*)data;
    int err;
    {
        cmdst_wifi_get para;
        err = setterApi->cmd(CMD_CODE_getWifiInfo ,&para ,sizeof(para));
        if(!err){
            wifiInfo->wifiSetting.enable = para.wifiEnable;
            wifiInfo->wifiSetting.channel = para.channel;
            wifiInfo->wifiSetting.encryption = para.encryption;
            wifiInfo->wifiSetting.type = para.netType;
            wifiInfo->wifiSetting.wepKeyId = para.wepKeyId;
            memcpy(wifiInfo->wifiSetting.ssid ,para.ssid ,32);
            memcpy(wifiInfo->wifiSetting.password ,para.pwd ,64);
        }else
            return err;
    }
    {
        cmdst_wifi_status para;
        err = setterApi->cmd(CMD_CODE_getWifiStatus ,&para ,sizeof(para));
        if(!err){
            if(1 != para){
                return DeviceStruct::ERR_wifi_have_not_been_inited;
            }
        }else
            return err;
    }
    {
        cmdst_aplist_get para;
        err = setterApi->cmd(CMD_CODE_getApList ,&para ,sizeof(para));
        if(!err){
            for(int i = 0 ;i < NUM_OF_APLIST ;i++){
                memcpy(wifiInfo->aplist[i].ssid ,para.aplist[i].ssid ,33);
                wifiInfo->aplist[i].encryption = para.aplist[i].encryption;
            }
        }
    }
    return err;
}

int Setter::setSoftAp(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    struct_softAp* softap = (struct_softAp*)data;
    int err;
    if(!isLogined)
        return DeviceStruct::ERR_no_logined;

    cmdst_softap para;
    memset(&para ,0 ,sizeof(para));
    para.wifiEnable = softap->enable;
    memcpy(para.ssid ,softap->ssid ,32);
    memcpy(para.pwd ,softap->password ,64);
    err = setterApi->cmd(CMD_CODE_set_softAp ,&para ,sizeof(para));
    return err;
}

int Setter::getSoftAp(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    struct_softAp* softap = (struct_softAp*)data;
    int err;
    cmdst_softap para;
    err = setterApi->cmd(CMD_CODE_getApList ,&para ,sizeof(para));
    if(!err){
        softap->enable = para.wifiEnable;
        memcpy(softap->ssid ,para.ssid ,32);
        memcpy(softap->password ,para.pwd ,64);
    }
    return err;
}

int Setter::setIpV4(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    struct_ipv4* ip = (struct_ipv4*)data;
    int err;
    if(!isLogined)
        return DeviceStruct::ERR_no_logined;

    net_info_st para;
    memset(&para ,0 ,sizeof(para));
    para.IPMode = ip->mode;
    para.IPAddressMode = ip->addressMode;
    for(int i = 0 ;i < 4 ;i++){
        para.IPAddress[i] = ip->address[i];
        para.GatewayAddress[i] = ip->gatewayAddress[i];
        para.SubnetMask[i] = ip->subnetMask[i];
    }
    err = setterApi->cmd(CMD_CODE_set_softAp ,&para ,sizeof(para));
    return err;
}

int Setter::getIpV4(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    struct_ipv4* ip = (struct_ipv4*)data;
    int err;
    net_info_st para;
    err = setterApi->cmd(CMD_CODE_get_softAp ,&para ,sizeof(para));
    if(!err){
        ip->mode = para.IPMode;
        ip->addressMode = para.IPAddressMode;
        for(int i = 0 ;i < 4 ;i++){
            ip->address[i] = para.IPAddress[i];
            ip->gatewayAddress[i] = para.GatewayAddress[i];
            ip->subnetMask[i] = para.SubnetMask[i];
        }
    }
    return err;
}

int Setter::setSaveTime(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    int* st = (int*)data;
    int err;
    if(!isLogined)
        return DeviceStruct::ERR_no_logined;

    cmdst_PSave_time para;
    para = (UINT8)*st;
    err = setterApi->cmd(CMD_CODE_set_softAp ,&para ,sizeof(para));
    return err;
}

int Setter::getSaveTime(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    int* st = (int*)data;
    int err;
    cmdst_PSave_time para;
    err = setterApi->cmd(CMD_CODE_get_softAp ,&para ,sizeof(para));
    if(!err){
        *st = para;
    }
    return err;
}
