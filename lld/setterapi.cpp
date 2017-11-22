#include "setterapi.h"
#include "setterstruct.h"
#include "deviceio.h"
#include "device.h"
using namespace JK;
#include <stdio.h>
#define C_LOG printf
#define _Q_LOG printf
#include <unistd.h>
#define delay100ms(x ,y) {if(y) usleep((x)  * 100 * 1000);}

#pragma pack(1)
typedef struct _COMM_HEADER
{
    UINT32 magic;
    UINT16 id;
    UINT16 len;     // lenght of data structure and its data

    // data structure
    UINT8 subid;
    UINT8 len2;

    // data append
    UINT8 subcmd;
}COMM_HEADER;

#define     MAGIC_NUM           0x1A2B3C4D
#define change_32bit_edian(x) (((x) << 24 & 0xff000000) | (((x) << 8) & 0x00ff0000) | (((x) >> 8) & 0x0000ff00) | (((x) >> 24) & 0xff))
#define change_16bit_edian(x) (((x) << 8) & 0xff00 | ((x) >> 8) & 0x00ff)
static const unsigned char INIT_VALUE = 0xfe;

SetterApi::SetterApi()
{

}

void SetterApi::install(DeviceIO* dio)
{
    this->dio = dio;
}

static int cmdParser(int& cmd ,int& direct ,int& data_buffer_size)
{
    int ret = 0;
    switch(cmd){
    case CMD_CODE_copy_get:             direct = 0; data_buffer_size = sizeof(copycmdset); break;//128
    case CMD_CODE_copy:                 direct = 1; data_buffer_size = sizeof(copycmdset); break;//128

    case CMD_CODE_getWifiInfo:          direct = 0; data_buffer_size = sizeof(cmdst_wifi_get); break;//180
    case CMD_CODE_setWifiInfo_noRead:   cmd = CMD_CODE_setWifiInfo;
    case CMD_CODE_setWifiInfo:          direct = 1; data_buffer_size = sizeof(cmdst_wifi_get); break;//180
    case CMD_CODE_getApList:            direct = 0; data_buffer_size = sizeof(cmdst_aplist_get); break;//340
    case CMD_CODE_getWifiStatus:        direct = 0; data_buffer_size = sizeof(cmdst_wifi_status); break;//1
    case CMD_CODE_get_softAp:           direct = 0; data_buffer_size = sizeof(cmdst_softap); break;//180
    case CMD_CODE_set_softAp:           direct = 1; data_buffer_size = sizeof(cmdst_softap); break;//180

    case CMD_CODE_getPsaveTime:         direct = 0; data_buffer_size = sizeof(cmdst_PSave_time);  break;//1
    case CMD_CODE_setPsaveTime:         direct = 1; data_buffer_size = sizeof(cmdst_PSave_time);  break;//1
    case CMD_CODE_get_userConfig:       direct = 0; data_buffer_size = sizeof(cmdst_userconfig);  break;//16
    case CMD_CODE_set_userConfig:       direct = 1; data_buffer_size = sizeof(cmdst_userconfig);  break;//16
    case CMD_CODE_setPasswd:            direct = 1; data_buffer_size = sizeof(cmdst_passwd);  break;//32
    case CMD_CODE_getPasswd:            direct = 0; data_buffer_size = sizeof(cmdst_passwd);  break;//32
    case CMD_CODE_confirmPasswd:        direct = 1; data_buffer_size = sizeof(cmdst_passwd);  break;//32
    case CMD_CODE_getPowerOff:          direct = 0; data_buffer_size = sizeof(cmdst_powerOff_time); break;//1
    case CMD_CODE_setPowerOff:          direct = 1; data_buffer_size = sizeof(cmdst_powerOff_time); break;//1
    case CMD_CODE_fusingScReset:        direct = 1; data_buffer_size = sizeof(cmdst_fusingScReset); break;//1
    case CMD_CODE_getTonerEnd:          direct = 0; data_buffer_size = sizeof(cmdst_tonerEnd); break;//1
    case CMD_CODE_setTonerEnd:          direct = 1; data_buffer_size = sizeof(cmdst_tonerEnd); break;//1

    case CMD_CODE_getv4:                direct = 0; data_buffer_size = sizeof(net_info_st);  break;//128
    case CMD_CODE_setv4:                direct = 1; data_buffer_size = sizeof(net_info_st);  break;//128
    case CMD_CODE_getv6:                direct = 0; data_buffer_size = sizeof(net_ipv6_st);  break;//360
    case CMD_CODE_setv6:                direct = 1; data_buffer_size = sizeof(net_ipv6_st);  break;//360

    default:        ret = -1;        break;
    }
    return ret;
}

int SetterApi::cmd(int cmd ,void* data ,int data_size)
{
    if(!data)
        return ERR_invalid_data;

    int device_cmd = cmd;
    int direct=0 ,data_buffer_size=0;
    int err = cmdParser(device_cmd ,direct ,data_buffer_size);
    if(err)
        return ERR_cmd_cannot_support;//not support

    int main_cmd ,sub_cmd;
    main_cmd = GET_CMDCODE(device_cmd);
    sub_cmd = GET_SUBCMDCODE(device_cmd);

    C_LOG("cmd:0x%x,main cmd:0x%x ,sub cmd:0x%x\n" ,cmd ,main_cmd ,sub_cmd);
    int device_cmd_len = sizeof(COMM_HEADER) + data_buffer_size;
    char* buffer = new char[device_cmd_len];
    memset( buffer, INIT_VALUE, sizeof(device_cmd_len));
    COMM_HEADER* ppkg = reinterpret_cast<COMM_HEADER*>(buffer);
    ppkg->magic = MAGIC_NUM;
    ppkg->id = main_cmd;
    ppkg->len = 3 + data_buffer_size * direct;
    // For the simple data setting, e.g. copy/scan/prn/wifi/net, SubID is always 0x13, len is always 0x01,
    // it just stand for the sub id. The real data length is defined by the lib
    ppkg->subid = 0x13;
    ppkg->len2 = 1;
    ppkg->subcmd = sub_cmd;
    if(direct){//set
        memcpy(buffer + sizeof(COMM_HEADER) ,data ,data_size);
    }

    if(cmd == CMD_CODE_setWifiInfo_noRead){
        err = write(buffer ,sizeof(COMM_HEADER)+data_buffer_size * direct);
    }else{
        err = writeThenRead(buffer ,sizeof(COMM_HEADER)+data_buffer_size * direct
                                               ,buffer ,sizeof(COMM_HEADER)+data_buffer_size * (1 - direct));
    }
    //check result
    if(!err && MAGIC_NUM == ppkg->magic){//ACK
        if(!direct){//get
            memcpy(data ,buffer + sizeof(COMM_HEADER) ,data_size);
        }else{//set
            if(cmd == CMD_CODE_setWifiInfo_noRead)
                err = ERR_WIFI_SET_SSID;
            else
                err = ppkg->subcmd;
        }
    }else
        err = -1;
    delete [] buffer;
    return err;
}

int SetterApi::write(char* wrBuffer ,int wrSize)
{
    int err = ERR_communication;
    int _write_size = 0;
    int i;
    char writeBuffer[wrSize];
    memcpy(writeBuffer ,wrBuffer ,wrSize);
    if(DeviceIO::Type_usb == dio->type()){
    //    err = _wake_up();
    //    if(err)
    //        return err;
    //    else
    //        delay100ms (10 ,true);

        int _read_size = 0;
        char readBuffer[0x3ff];

    //    bool delay = false;
        for(i = 0 ;i < 1 ;i++){
    //    for(i = 0 ;i < 5 ;i++){
            _read_size = dio->read(readBuffer ,0x3ff);
            C_LOG("before write clear read buffer :%d" ,_read_size);

    //        delay100ms (0.3 ,delay);

            _write_size = dio->write(writeBuffer ,wrSize);
            delay100ms (1 ,true);
            C_LOG("write size:%d......%d" ,wrSize ,_write_size);
    //        if(_write_size == wrSize){
                err = ERR_ACK;
                break;
    //        }else{
    //            err = ERR_communication;
    //        }
        }
    //    if(err)
    //        _Q_LOG("write wrong");
    }else{

        for(i = 0 ;i < 1 ;i++){
    //    for(i = 0 ;i < 3 ;i++){
            _write_size = write(writeBuffer ,wrSize);
            C_LOG("write size:%d......%d" ,wrSize ,_write_size);
            if(_write_size == wrSize){
                err = ERR_ACK;
                break;
            }
        }
    }
    return err;
}

int SetterApi::writeThenRead(char* wrBuffer ,int wrSize ,char* rdBuffer ,int rdSize)
{
    bool ifdelay = dio->type() == DeviceIO::Type_usb;
    int err = write(wrBuffer ,wrSize);
//    if(1){
    if(!err){
        int j;
        int _read_size = 0;
        int nocheck=0;

        delay100ms(9 ,ifdelay);
        for(j = 0 ;j < 50 ;j++){
            if(!nocheck){
                if(1 == dio->read(rdBuffer,1)){
                    if(0x4d != rdBuffer[0]){
                        C_LOG("waiting for 0x4d:%#.2x" ,rdBuffer[0]);
                        delay100ms (1 ,ifdelay);
                        continue;
                    }
                }else{
                    _Q_LOG("cannot read now,wait 100 ms read again");
                    delay100ms (1 ,ifdelay);
                    continue;
                }
            }
            nocheck = 0;
            delay100ms (1 ,ifdelay);
            if(1 == dio->read(rdBuffer+1,1)){
                if(0x3c == rdBuffer[1]){
                    delay100ms (1 ,ifdelay);
                    _read_size = dio->read(rdBuffer+2 ,rdSize-2);
                    C_LOG("read size:%d" ,_read_size == -1 ?-1 : _read_size + 2);
                    j++;
                    break;
                }else if(0x4d == rdBuffer[1]){
                    nocheck = 1;
                }
            }
            delay100ms (1 ,ifdelay);
        }

        C_LOG("try %d times" ,j);
        if(_read_size == rdSize -2){
            err = ERR_ACK;
        }else{
            _Q_LOG("read wrong");
            err = ERR_communication;
        }
    }
    return err;
}
