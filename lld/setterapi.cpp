#include "setterapi.h"
#include "setterstruct.h"
#include "deviceio.h"
//#include "device.h"
#include "../platform/log.h"
#include "../platform/devicestruct.h"
using namespace JK;
#include <string.h>
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

//#define MAGIC_NUM 0x4d3c2b1a
#define MAGIC_NUM 0x1A2B3C4D
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
    case CMD_CODE_copy_get:             direct = 0; data_buffer_size = 128; break;//128
    case CMD_CODE_copy:                 direct = 1; data_buffer_size = 128; break;//128

    case CMD_CODE_getWifiInfo:          direct = 0; data_buffer_size = 180; break;//180
    case CMD_CODE_setWifiInfo_noRead:   cmd = CMD_CODE_setWifiInfo;
    case CMD_CODE_setWifiInfo:          direct = 1; data_buffer_size = 180; break;//180
    case CMD_CODE_getApList:            direct = 0; data_buffer_size = 340; break;//340
    case CMD_CODE_getWifiStatus:        direct = 0; data_buffer_size = 1; break;//1
    case CMD_CODE_get_softAp:           direct = 0; data_buffer_size = 180; break;//180
    case CMD_CODE_set_softAp:           direct = 1; data_buffer_size = 180; break;//180
    case CMD_CODE_getSoftApList:           direct = 0; data_buffer_size = 340; break;//340

    case CMD_CODE_getPsaveTime:         direct = 0; data_buffer_size = 1;  break;//1
    case CMD_CODE_setPsaveTime:         direct = 1; data_buffer_size = 1;  break;//1
    case CMD_CODE_get_userConfig:       direct = 0; data_buffer_size = 16;  break;//16
    case CMD_CODE_set_userConfig:       direct = 1; data_buffer_size = 16;  break;//16
    case CMD_CODE_setPasswd:            direct = 1; data_buffer_size = 32;  break;//32
    case CMD_CODE_getPasswd:            direct = 0; data_buffer_size = 32;  break;//32
    case CMD_CODE_confirmPasswd:        direct = 1; data_buffer_size = 32;  break;//32
    case CMD_CODE_getPowerOff:          direct = 0; data_buffer_size = 1; break;//1
    case CMD_CODE_setPowerOff:          direct = 1; data_buffer_size = 1; break;//1
    case CMD_CODE_fusingScReset:        direct = 1; data_buffer_size = 1; break;//1
    case CMD_CODE_getTonerEnd:          direct = 0; data_buffer_size = 1; break;//1
    case CMD_CODE_setTonerEnd:          direct = 1; data_buffer_size = 1; break;//1

    case CMD_CODE_getv4:                direct = 0; data_buffer_size = 128;  break;//128
    case CMD_CODE_setv4:                direct = 1; data_buffer_size = 128;  break;//128
    case CMD_CODE_getv6:                direct = 0; data_buffer_size = 360;  break;//360
    case CMD_CODE_setv6:                direct = 1; data_buffer_size = 360;  break;//360

    default:        ret = -1;        break;
    }
    return ret;
}

int SetterApi::_cmd(int cmd ,void* data ,int data_size)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;

    int device_cmd = cmd;
    int direct=0 ,data_buffer_size=0;
    int err = cmdParser(device_cmd ,direct ,data_buffer_size);
    if(err)
        return DeviceStruct::ERR_cmd_cannot_support;//not support

    int main_cmd ,sub_cmd;
    main_cmd = GET_CMDCODE(device_cmd);
    sub_cmd = GET_SUBCMDCODE(device_cmd);

    LOG_PARA("cmd:0x%x,main cmd:0x%x ,sub cmd:0x%x\n" ,cmd ,main_cmd ,sub_cmd);
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
//    if(!err){//ACK 0x1A2B3C4D
    if(!err && 0x1A2B3C4D == ppkg->magic){//ACK 0x1A2B3C4D
//    if(!err && MAGIC_NUM == ppkg->magic){//ACK 0x1A2B3C4D
        if(!direct){//get
            memcpy(data ,buffer + sizeof(COMM_HEADER) ,data_size);
        }else{//set
            if(cmd == CMD_CODE_setWifiInfo_noRead)
                err = DeviceStruct::ERR_WIFI_SET_SSID;
            else
                err = ppkg->subcmd;
        }
    }else
        err = -1;
    delete [] buffer;
    return err;
}

int SetterApi::cmd(int cmd ,void* data ,int data_size)
{
    int err;
    int times = 1;
    do{
        err = dio->open(1);
        if(!err){
            err = _cmd(cmd ,data ,data_size);
            dio->close();
        }
        if(!err)
            break;
        if(--times == 0){
            break;
        }else{
            usleep(500 * 1000);
        }
    }while(true);
//    for(int i = 0 ;i < 1 ;i++){
//        err = dio->open(1);
//        if(!err){
//            err = _cmd(cmd ,data ,data_size);
//            dio->close();
//        }
//        if(!err)
//            break;
//        usleep(500 * 1000);
//    }
    return err;
}

#if 1

int SetterApi::write(char* wrBuffer ,int wrSize)
{
    int err = DeviceStruct::ERR_communication;
    int result;
    if(wrSize > 8){
        result = dio->writeCommand(wrBuffer ,8);
        if(result > 0){
            result = dio->writeCommand(wrBuffer + 8 ,wrSize - 8);
        }
    }
    char* writeBuffer = wrBuffer;
    LOG_PARA("write command:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x"
              ,writeBuffer[0] ,writeBuffer[1] ,writeBuffer[2] ,writeBuffer[3] ,writeBuffer[4]
             ,writeBuffer[5] ,writeBuffer[6] ,writeBuffer[7] ,writeBuffer[8] ,writeBuffer[9]
             ,writeBuffer[10]);
    if(result > 0){
        err = DeviceStruct::ERR_ACK;
    }
    return err;
}

int SetterApi::writeThenRead(char* wrBuffer ,int wrSize ,char* rdBuffer ,int rdSize)
{
    int err = write(wrBuffer ,wrSize);
    int result;
    if(!err){
        delay100ms(1 ,true);
        result = dio->readCommand(rdBuffer ,rdSize);
        if(result != rdSize){
            err = DeviceStruct::ERR_communication;
        }
    }
    return err;
}
#else

int SetterApi::write(char* wrBuffer ,int wrSize)
{
    int err = DeviceStruct::ERR_communication;
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

//        int _read_size = 0;
//        char readBuffer[0x3ff];

    //    bool delay = false;

        for(i = 0 ;i < 1 ;i++){
    //    for(i = 0 ;i < 5 ;i++){
//            _read_size = dio->readCommand(readBuffer ,0x3ff);
//            LOG_PARA("before write clear read buffer :%d" ,_read_size);

    //        delay100ms (0.3 ,delay);
            if(wrSize > 8){

                char cmd[8] = { 0x1a,0x2b,0x3c,0x4d, 6, 1, 3, 0 };
                _write_size = dio->write(cmd ,8);
                if(_write_size >= 0){
                    cmd[0] = 0x13;
                    cmd[1] = 1;
                    cmd[2] = 0;
                    _write_size = dio->write(cmd ,3);
                }

//                _write_size = dio->writeCommand(writeBuffer ,8);
//                LOG_PARA("write size:%d......%d" ,8 ,_write_size);
//                _write_size = dio->writeCommand(writeBuffer + 8 ,wrSize - 8);
//                LOG_PARA("write size:%d......%d" ,wrSize - 8 ,_write_size);
            }
//            _write_size = dio->writeCommand(writeBuffer ,wrSize);
//            LOG_PARA("write size:%d......%d" ,wrSize ,_write_size);
            delay100ms (1 ,true);
            err = DeviceStruct::ERR_ACK;
            break;
    //        if(_write_size == wrSize){
    //        }else{
    //            err = DeviceStruct::ERR_communication;
    //        }
        }
    //    if(err)
    //        LOG_NOPARA("write wrong");
    }else{
        for(i = 0 ;i < 1 ;i++){
    //    for(i = 0 ;i < 3 ;i++){
            _write_size = dio->writeCommand(writeBuffer ,wrSize);
            LOG_PARA("write size:%d......%d" ,wrSize ,_write_size);
            if(_write_size == wrSize){
                err = DeviceStruct::ERR_ACK;
                break;
            }
        }
    }
    LOG_PARA("write command:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x"
              ,writeBuffer[0] ,writeBuffer[1] ,writeBuffer[2] ,writeBuffer[3] ,writeBuffer[4]
             ,writeBuffer[5] ,writeBuffer[6] ,writeBuffer[7] ,writeBuffer[8] ,writeBuffer[9]
             ,writeBuffer[10]);
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
        for(j = 0 ;j < 1 ;j++){
            if(!nocheck){
                if(1 == dio->readCommand(rdBuffer,1)){
                    if(0x4d != rdBuffer[0]){
                        LOG_PARA("waiting for 0x4d:%#.2x" ,rdBuffer[0]);
                        delay100ms (1 ,ifdelay);
                        continue;
                    }
                }else{
                    LOG_NOPARA("cannot read now,wait 100 ms read again");
                    delay100ms (1 ,ifdelay);
                    continue;
                }
            }
            nocheck = 0;
            delay100ms (1 ,ifdelay);
            if(1 == dio->readCommand(rdBuffer+1,1)){
                if(0x3c == rdBuffer[1]){
                    delay100ms (1 ,ifdelay);
                    _read_size = dio->readCommand(rdBuffer+2 ,rdSize-2);
                    LOG_PARA("read size:%d" ,_read_size == -1 ?-1 : _read_size + 2);
                    j++;
                    break;
                }else if(0x4d == rdBuffer[1]){
                    nocheck = 1;
                }
            }
            delay100ms (1 ,ifdelay);
        }

//        LOG_PARA("try %d times" ,j);
        if(_read_size == rdSize -2){
            err = DeviceStruct::ERR_ACK;
        }else{
            LOG_NOPARA("read wrong");
            err = DeviceStruct::ERR_communication;
        }
    }
    return err;
}
#endif
