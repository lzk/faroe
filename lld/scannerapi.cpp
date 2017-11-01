#include "scannerapi.h"
#include "deviceio.h"
#include <string.h>
using namespace JK;
#define NO_DIO -10

ScannerAPI::ScannerAPI()
{

}
void ScannerAPI::install(DeviceIO* dio)
{
    this->dio = dio;
}

int ScannerAPI::jobCreate()
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
    pCmdData[4] = JOB_ADF;//0;

    SC_JOB_STA_T status;

    memset(&status ,0 ,sizeof(status));
    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && status.ack == 'E') {
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

    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E' || status.id != JobID)) {
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

    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E' || status.id != JobID)) {
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

    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E' || status.id != JobID)) {
        result = -1;
    }
    return result<0?-1:0;
}

int ScannerAPI::setParameters(const SC_PAR_DATA_T& para)
{

    if(dio == NULL)
        return NO_DIO;

    SC_PAR_T cmdData;
    char* pCmdData = (char*)&cmdData;
    SC_PAR_T* cmd = &cmdData;
    memset(pCmdData ,0 ,sizeof(cmdData));
    pCmdData[0] = 'P';
    pCmdData[1] = 'A';
    pCmdData[2] = 'R';

    SC_PAR_STA_T		status;

    cmd->id = JobID;
    cmd->length	= sizeof(SC_PAR_DATA_T);
    memset(&status ,0 ,sizeof(status));

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->write((char*)&para, sizeof(para));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));
    else
        return -1;
    if(result>=0 && (status.ack == 'E' || status.id != JobID)) {
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

    cmd->id = JobID;
    cmd->length			= sizeof(SC_INFO_DATA_T);
    memset(&sc_infodata ,0 ,sizeof(sc_infodata));

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&sc_infodata, sizeof(sc_infodata));

    char* code = (char*)&sc_infodata.code;
    if(result >= 0 &&
       (code[0] != 'I' || code[1] != 'D' || code[2] != 'A' || code[3] != 'T'
       || sc_infodata.Cancel)){
//            (sc_infodata.code != 1413563465)){
//        ((('I'&0x000000ff)<<24) | (('D'&0x000000ff)<<16) | (('A'&0x000000ff)<<8) | 'T') || sc_infodata.Cancel)) {
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
    if(cmd->length > 0x100000) //for GL cmd
        cmd->length = 0x100000;

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read(buffer, cmd->length);

    if(result>=0) {
        *ImgSize = result;
        result = 0;
    }
    return result;
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

    cmd->id = JobID;
    memset(&status ,0 ,sizeof(status));

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E' || status.id != JobID)) {
        result = -1;
    }
    return result<0?-1:0;
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

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->write(buffer, size);
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E')) {
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

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

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

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E')) {
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

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

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

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

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

    int result = dio->write((char*)cmd, sizeof(*cmd));
    if(result>=0)
        result = dio->read((char*)&status, sizeof(status));

    if(result>=0 && (status.ack == 'E')) {
        result = -1;
    }
    return result<0?-1:0;
}
