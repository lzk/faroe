#include "scannerapi.h"
#include "deviceio.h"
#include "../platform/log.h"
#include <string.h>
#include <unistd.h>
using namespace JK;
#define NO_DIO -10
#define USLEEP_TIME 0
#define DELAY(x) usleep(x)
//#define DELAYTIMES() DELAY(10000)
#define DELAYTIMES()

ScannerAPI::ScannerAPI()
{

}
void ScannerAPI::install(DeviceIO* dio)
{
    this->dio = dio;
}

bool ScannerAPI::ready()
{
    if(dio->type() == DeviceIO::Type_usb)
        return true;
    int result = 0;
    char status = 0;
    result = dio->read(&status ,1);
    return result == 1 && status == 1;
}

int ScannerAPI::lock()
{
    if(dio == NULL)
        return NO_DIO;

    if(dio->type() == DeviceIO::Type_usb){
        return 0;
    }

    U8 cmd[8] = { 'L','O','C','K', 0, 0, 0, 0 };
    U8 ack[8] = { 0 };

    int result = dio->write((char*)cmd, sizeof(cmd));
    if(result>=0){
        result = dio->read((char*)ack, sizeof(ack));
    }
    if(result>=0 &&
            (ack[0] == 'S' &&
             ack[1] == 'T' &&
             ack[2] == 'A' &&
             ack[4] == 'A')) {
        result = 0;
    }
    return result<0?-1:0;
}

int ScannerAPI::unlock()
{
    if(dio == NULL)
        return NO_DIO;

    if(dio->type() == DeviceIO::Type_usb){
        return 0;
    }

    U8 cmd[8] = { 'U','L','C','K', 0, 0, 0, 0 };
    U8 ack[8] = { 0 };

    int result = dio->write((char*)cmd, sizeof(cmd));
    if(result>=0){
        result = dio->read((char*)ack, sizeof(ack));
    }
    if(result>=0 &&
            (ack[0] == 'S' &&
             ack[1] == 'T' &&
             ack[2] == 'A')) {
        result = 0;
    }
    return result<0?-1:0;
}

//Job type define
#define JOB_PULL_SCAN			0x01
#define JOB_PULL_SCAN_BUTTON	0x02
#define JOB_WIFI_SCAN			0x03
#define JOB_PUSH_STORAGE		0x04
#define JOB_PUSH_FTP			0x05
#define JOB_PUSH_SMB			0x06
int ScannerAPI::jobCreate(int source)
{
    if(dio == NULL)
        return NO_DIO;
    SC_JOB_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_JOB_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'J';
    pCmdData[1] = 'O';
    pCmdData[2] = 'B';
    pCmdData[4] = source;

    if(dio->type() == DeviceIO::Type_usb){
        pCmdData[7] = JOB_PULL_SCAN;
    }else{
        pCmdData[7] = JOB_WIFI_SCAN;
    }

    SC_JOB_STA_T status;

    memset(&status ,0 ,sizeof(status));
    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }
    if(result>=0 &&
            status.ack == 'E'){
        return status.err;
    }

    JobID = status.id;

    return result<0?-1:0;
}

int ScannerAPI::jobEnd()
{
    if(dio == NULL)
        return NO_DIO;

    SC_JOB_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_JOB_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'J';
    pCmdData[1] = 'O';
    pCmdData[2] = 'B';
    pCmdData[4] = 'E';

    SC_JOB_STA_T status;

//    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }
    if(result<=0
            || status.ack == 'E'
//            || status.id != JobID
            ){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::startScan()
{
    if(dio == NULL)
        return NO_DIO;

    SC_SCAN_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_SCAN_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'S';
    pCmdData[1] = 'C';
    pCmdData[2] = 'A';
    pCmdData[3] = 'N';

    SC_SCAN_STA_T		status;

//    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0
            || status.ack == 'E'
//            || status.id != JobID
            ){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::stopScan()
{
    if(dio == NULL)
        return NO_DIO;

    SC_STOP_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_STOP_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'S';
    pCmdData[1] = 'T';
    pCmdData[2] = 'O';
    pCmdData[3] = 'P';

    SC_STOP_STA_T		status;

//    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0
            || status.ack == 'E'
//            || status.id != JobID
            ){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::setParameters(const SC_PAR_DATA_T& para)
{

    if(dio == NULL)
        return NO_DIO;
    char test[88];
    memcpy(test ,&para ,sizeof(test));
    SC_PAR_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_PAR_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'P';
    pCmdData[1] = 'A';
    pCmdData[2] = 'R';

    SC_PAR_STA_T		status;

//    cmd->id = JobID;
    cmd->length	= sizeof(SC_PAR_DATA_T);
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->write((char*)&para, sizeof(para));
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }
    else{
        LOG_NOPARA("setParameters write err!");
        return -1;
    }
    if(result<= 0
            || status.ack == 'E'
//            || status.id != JobID
//            (status.code != CODE_STA || status.ack != 'A')
            ){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::getInfo(SC_INFO_DATA_T& sc_infodata)
{
    if(dio == NULL)
        return NO_DIO;

    SC_INFO_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_INFO_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'I';
    pCmdData[1] = 'N';
    pCmdData[2] = 'F';
    pCmdData[3] = 'O';

//    cmd->id = JobID;
    cmd->length			= sizeof(SC_INFO_DATA_T);
    memset(&sc_infodata ,0 ,sizeof(sc_infodata));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&sc_infodata, sizeof(sc_infodata));
    }

    char* code = (char*)&sc_infodata.code;
    if(result <= 0
       ||(code[0] != 'I' || code[1] != 'D' || code[2] != 'A' || code[3] != 'T')
       || sc_infodata.ErrorStatus.scan_canceled_err){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::cancelScan()
{
    if(dio == NULL)
        return NO_DIO;

    SC_CNL_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_CNL_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'C';
    pCmdData[1] = 'A';
    pCmdData[2] = 'N';
    pCmdData[3] = 'C';

    SC_CNL_STA_T		status;

//    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0
            || status.ack == 'E'
//            || status.id != JobID
            ){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::readScan(int dup, int *ImgSize,char* buffer ,int size)
{

    if(dio == NULL)
        return NO_DIO;

    SC_IMG_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_IMG_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'I';
    pCmdData[1] = 'M';
    pCmdData[2] = 'G';

    cmd->side = dup;
    cmd->length = size;
    if(cmd->length > MAX_SCAN_LENGTH) //for GL cmd
        cmd->length = MAX_SCAN_LENGTH;

//    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
//        DELAYTIMES();
        result = dio->read(buffer, cmd->length);
    }

    if(result>=0) {
        *ImgSize = result;
        result = 0;
    }
    return result;
}

int ScannerAPI::nvramW(int address ,char* buffer ,int size)
{
    if(dio == NULL)
        return NO_DIO;

    S_NVRW cmdData;
    char* pCmdData = (char*)&cmdData;
    S_NVRW* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'N';
    pCmdData[1] = 'V';
    pCmdData[2] = 'W';

    S_NVRW_status		status;

    cmd->straddr = address;
    cmd->length = size;
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->write(buffer, size);
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0 || (status.ack == 'E')) {
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::nvramR(int address ,char* buffer ,int size)
{
    if(dio == NULL)
        return NO_DIO;

    S_NVRW cmdData;
    char* pCmdData = (char*)&cmdData;
    S_NVRW* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'N';
    pCmdData[1] = 'V';
    pCmdData[2] = 'R';

    S_NVRW_status		status;

    cmd->straddr = address;
    cmd->length = size;
    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result>=0 && (status.ack == 'E')) {
        result = -1;
    }
    if(result >= 0)
        result = dio->read(buffer, size);
    return result;//<0?-1:0;
}

int ScannerAPI::adfCheck(SC_ADF_CHECK_STA_T& status)
{
    if(dio == NULL)
        return NO_DIO;

    SC_ADF_CHECK_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_ADF_CHECK_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'A';
    pCmdData[1] = 'D';
    pCmdData[2] = 'F';

    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0 || (status.ack == 'E')) {
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::getFwVersion(char* version ,int maxLength)
{
    if(dio == NULL)
        return NO_DIO;

    fw_version_ack cmdData;
    char* pCmdData = (char*)&cmdData;
    fw_version_ack* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'V';
    pCmdData[1] = 'E';
    pCmdData[2] = 'R';
    pCmdData[3] = 'N';

    fw_version_get		status;

    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result>=0 && (status.check != 'E') && maxLength > status.length) {
        result = dio->read(version, status.length);
        version[status.length] = 0;
    }
    return result;//<0?-1:0;
}

int ScannerAPI::getStatus(SC_STATUS_DATA_T& status)
{
    if(dio == NULL)
        return NO_DIO;

    SC_STATUS_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_STATUS_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'S';
    pCmdData[1] = 'T';
    pCmdData[2] = 'A';
    pCmdData[3] = 'S';

    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    return result<0?-1:0;
}

int ScannerAPI::resetScan()
{
    if(dio == NULL)
        return NO_DIO;

    SC_SCAN_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_SCAN_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'R';
    pCmdData[1] = 'S';
    pCmdData[2] = 'E';
    pCmdData[3] = 'T';

    SC_SCAN_STA_T		status;

    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0 || (status.ack == 'E')) {
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::getPowerSupply(SC_POWER_INFO_T& sc_powerData)
{
    if(dio == NULL)
        return NO_DIO;

    SC_POWER_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_POWER_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'P';
    pCmdData[1] = 'W';
    pCmdData[2] = 'R';
    pCmdData[3] = 'M';

    memset(&sc_powerData, 0, sizeof(sc_powerData));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&sc_powerData, sizeof(sc_powerData));
    }

    char* code = (char*)&sc_powerData.code;
    if(result <= 0
       ||(code[0] != 'S' || code[1] != 'T' || code[2] != 'A')
            || sc_powerData.ack != 'A'
            ){
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::setGamma(const unsigned int* gammaTable ,int length)
{
    if(dio == NULL)
        return NO_DIO;

    SC_PAR_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_PAR_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'G';
    pCmdData[1] = 'A';
    pCmdData[2] = 'M';
    pCmdData[3] = 'A';
    cmd->length = sizeof(gammaTable[0]) * length;

    SC_PAR_STA_T		status;

    memset(&status ,0 ,sizeof(status));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->write((char*)gammaTable, cmd->length);
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&status, sizeof(status));
    }

    if(result<=0 || (status.ack == 'E')) {
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::getPowerSaveTime(int& sleepTime ,int& offTime ,int& powerMode)
{
    if(dio == NULL)
        return NO_DIO;

    SC_PWRS_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_PWRS_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'P';
    pCmdData[1] = 'W';
    pCmdData[2] = 'R';
    pCmdData[3] = 'S';

    SC_GET_PWRS_DATA_T data;
    memset(&data, 0, sizeof(data));

    SC_PWRS_STA_T sta;
    memset(&sta, 0, sizeof(sta));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&sta, sizeof(sta));
    }
    char* code = (char*)&sta.code;
    if(result <= 0
       ||(code[0] != 'S' || code[1] != 'T' || code[2] != 'A')
            || sta.ack != 'A'
            ){
        result = -1;
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&data, sizeof(data));
    }

    if(data.disableAutoSleep & 0xff)
        sleepTime = 0;
    else
        sleepTime = data.autoSleepTime;
    if(data.disableAutoOff & 0xff)
        offTime = 0;
    else
        offTime = data.autoOffTime;
    powerMode = sta.powermodecode;
    return result<0?-1:0;

}

int ScannerAPI::setPowerSaveTime(int sleepTime ,int offTime ,int& powerMode)
{
    if(dio == NULL)
        return NO_DIO;

    SC_PWRS_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_PWRS_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'P';
    pCmdData[1] = 'W';
    pCmdData[2] = 'R';
    pCmdData[3] = 'S';
    cmd->option = 1;

    SC_GET_PWRS_DATA_T data;
    data.disableAutoOff = 0;
    data.disableAutoSleep = 0;
    data.autoOffTime = offTime;
    data.autoSleepTime = sleepTime;

    SC_PWRS_STA_T sta;
    memset(&sta, 0, sizeof(sta));

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->write((char*)&data, sizeof(data));
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)&sta, sizeof(sta));
    }
    char* code = (char*)&sta.code;
    if(result <= 0
       ||(code[0] != 'S' || code[1] != 'T' || code[2] != 'A')
            || sta.ack != 'A'
            ){
        result = -1;
    }

    powerMode = sta.powermodecode;
    return result<0?-1:0;
}

int ScannerAPI::nvram_read(unsigned char addr ,unsigned int len ,unsigned char* data)
{
    if(dio == NULL)
        return NO_DIO;

    char cmd[8] = { 'R','E','E','P' ,0,0,0,0};
    char status[8] = { 0 };
    cmd[4] = addr;
    cmd[5] = len;

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)status, sizeof(status));
    }
    char* code = (char*)status;
    if(result <= 0
       ||(code[0] != 'S' || code[1] != 'T' || code[2] != 'A')
            || code[4] != 'A'
            ){
        result = -1;
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)data, len);
    }
    return result<0?-1:0;
}

int ScannerAPI::nvram_write(unsigned char addr ,unsigned int len ,unsigned char* data)
{
    if(dio == NULL)
        return NO_DIO;

    char cmd[8] = { 'W','E','E','P' ,0,0,0,0};
    char status[8] = { 0 };
    cmd[4] = addr;
    cmd[5] = len;

    DELAYTIMES();
    int result = dio->write((char*)cmd, sizeof(cmd));
    if(result>=0){
        DELAYTIMES();
        result = dio->write((char*)data, len);
    }
    if(result>=0){
        DELAYTIMES();
        result = dio->read((char*)status, sizeof(status));
    }
    char* code = (char*)status;
    if(result <= 0
       ||(code[0] != 'S' || code[1] != 'T' || code[2] != 'A')
            || code[4] != 'A'
            ){
        result = -1;
    }
    return result<0?-1:0;
}
