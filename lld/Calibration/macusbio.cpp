#include "../deviceio.h"
using namespace JK;

DeviceIO* usbio;
int CMDIO_OpenDevice()
{
    return usbio->open() == 0;
}

int CMDIO_CloseDevice()
{
    return usbio->close() == 0;
}

int CMDIO_BulkWriteEx(int pipe, void *buffer, unsigned int dwLen)
{
    return usbio->write((char*)buffer ,dwLen) >= 0;
}

int CMDIO_BulkReadEx(int pipe, void *buffer, unsigned int dwLen)
{
    return usbio->read((char*)buffer ,dwLen) >= 0;
}

//bool isLittleEndian();
int GammaTransLTCtoGL(unsigned int *pbyRed, unsigned int *pbyGreen, unsigned int *pbyBlue, unsigned int *GLGamma)
{
//    bool littleEndian = isLittleEndian();
    for (int i = 0; i<256; i++)
    {
        if (i<255) {
//            if(littleEndian){
                GLGamma[i] = (pbyRed[i*256] & 0x0000ffff) | ((pbyRed[(i+1)*256] & 0x0000ffff) << 16);
//                GLGamma[i + 256] = (pbyGreen[i*256] & 0x0000ffff) | ((pbyGreen[(i+1)*256] & 0x0000ffff) << 16);
//                GLGamma[i + 256 * 2] = (pbyBlue[i*256] & 0x0000ffff) | ((pbyBlue[(i+1)*256] & 0x0000ffff) << 16);
//            }else{
//                GLGamma[i] = (pbyRed[i*256] & 0xffff0000)
//                        | ((pbyRed[(i+1)*256] & 0xffff0000) >> 16);
////                GLGamma[i + 256] = (pbyGreen[i*256] & 0xffff0000)
////                        | ((pbyGreen[(i+1)*256] & 0xffff0000) >> 16);
////                GLGamma[i + 512] = (pbyBlue[i*256] & 0xffff0000)
////                        | ((pbyBlue[(i+1)*256] & 0xffff0000) >> 16);
//            }
        }else{
//            if(littleEndian){
                GLGamma[i] = (pbyRed[i*256] & 0x0000ffff) | 0xffff0000;
//                GLGamma[i + 256] = (pbyGreen[i*256] & 0x0000ffff) | 0xffff0000;
//                GLGamma[i + 256 * 2] = (pbyBlue[i*256] & 0x0000ffff) | 0xffff0000;
//            }else{
//                GLGamma[i] = (pbyRed[i*256] & 0xffff0000) | 0x0000ffff;
////                GLGamma[i + 256] = (pbyGreen[i*256] & 0xffff0000) | 0x0000ffff;
////                GLGamma[i + 512] = (pbyBlue[i*256] & 0xffff0000) | 0x0000ffff;
//            }
        }
        GLGamma[i + 256] = GLGamma[i];
        GLGamma[i + 512] = GLGamma[i];
    }
}

#include <unistd.h>
void Sleep(int x)
{
    usleep(x * 1000);
}

//********************************************************************************************
//Do Calibration //Devid added 2017/10/30

#include "ScanCMD.h"
enum Scan_RET
{
    RETSCAN_OK = 0,
    RETSCAN_ERRORDLL = 1,
    RETSCAN_OPENFAIL = 2,
    RETSCAN_ERRORPARAMETER = 3,
    RETSCAN_NO_ENOUGH_SPACE = 5,
    RETSCAN_ERROR_PORT = 6,
    RETSCAN_CANCEL = 7,
    RETSCAN_BUSY = 8,
    RETSCAN_ERROR = 9,
    RETSCAN_OPENFAIL_NET = 10,
    RETSCAN_PAPER_JAM = 11,
    RETSCAN_COVER_OPEN = 12,
    RETSCAN_PAPER_NOT_READY = 13,
    RETSCAN_CREATE_JOB_FAIL = 14,
    RETSCAN_ADFCOVER_NOT_READY = 15,
    RETSCAN_HOME_NOT_READY = 16,
    RETSCAN_ULTRA_SONIC = 17,
    RETSCAN_ERROR_POWER1 = 18,
    RETSCAN_ERROR_POWER2 = 19,
    RETSCAN_JOB_MISSING = 20,
    RETSCAN_JOB_GOGING = 21,
    RETSCAN_TIME_OUT = 22,
    RETSCAN_USB_TRANSFERERROR = 23,
    RETSCAN_WIFI_TRANSFERERROR = 24,
    RETSCAN_ADFPATH_NOT_READY = 25,
    RETSCAN_ADFDOC_NOT_READY = 26,
    RETSCAN_GETINFO_FAIL = 27,
};

#define START_STAGE					0x1
#define SCANNING_STAGE				0x2
#define PUSH_TRANSFER_STAGE		0x3

extern void job_Set_Calibration_Par(unsigned char type, SC_PAR_T_ *_par);
extern int job_Calibration(SC_PAR_T_ *_par);
extern U32 read_from_ini(void);

extern SC_PAR_T_ sc_par;

U8 SCAN_DOC_SIZE = DOC_SIZE_FULL;
char IniFile[256], Profile[64];

extern Scan_RET ScannerStatusCheck_(char stage);

 int DoCalibration()
{
    int nResult = RETSCAN_OK;

    read_from_ini();

    if (!CMDIO_OpenDevice())
    {
        return RETSCAN_OPENFAIL;
    }

    job_Set_Calibration_Par(1, &sc_par);

    if (sc_par.acquire & ACQ_NO_PP_SENSOR) {

    }
    else {
        nResult = ScannerStatusCheck_(START_STAGE);
    }

//	nResult = ScannerStatusCheck_(START_STAGE);

    if (nResult == RETSCAN_OK)
    {
        nResult = job_Calibration(&sc_par);
//        int nCalibration = RETSCAN_OK;
//        nCalibration = job_Calibration(&sc_par);

//        if (!nCalibration)
//        {
//            nResult = RETSCAN_OK;
//        }
//        else
//        {
//            if (sc_par.acquire & ACQ_NO_PP_SENSOR) {

//            }
//            else {
//                nResult = ScannerStatusCheck_(SCANNING_STAGE);
//                //	nResult = RETSCAN_CANCEL;
//            }
//        }
    }

    CMDIO_CloseDevice();

    return nResult;
}
