#include "scanner.h"
#include <unistd.h>
#include <string.h>
#include "platformapp.h"
#include "scannerapi.h"
#include "../platform/log.h"
#include "device.h"
using namespace JK;

#define _SCANMODE_1BIT_BLACKWHITE 1
#define _SCANMODE_8BIT_GRAYSCALE  8
#define _SCANMODE_24BIT_COLOR     24

const Scanner::Setting defaultSetting = {
    .BitsPerPixel = IMG_BIT,
    .dpi_x=300,
    .dpi_y=300,
    .scan_doc_size=SETTING_DOC_SIZE_FULL,
    .contrast=50,
    .brightness=50,
    .ADFMode=SCAN_DUPLEX,
    .MultiFeed=true,
    .AutoCrop=true,
    .onepage=SCAN_PAGE,
    .source=CODE_ADF,
    .format=CODE_JPG,
};

Scanner::Scanner():
    pPlatformApp(NULL)
    ,scannerApi(new ScannerAPI)
    ,m_cancel(false)
{
    memset(&parameters ,0 ,sizeof(parameters));
    parameters.setting = defaultSetting;
}


Scanner::~Scanner()
{
    delete scannerApi;
}

void Scanner::install(DeviceIO* dio ,PlatformApp* platformApp)
{
    scannerApi->install(dio);
    pPlatformApp = platformApp;
}

void Scanner::cancel()
{
    m_cancel = true;
}

int Scanner::receiveData()
{
    SC_INFO_DATA_T sc_infodata;
    int dup;
    int duplex;
    int readBytes ,readTotalBytes[2] ,toReadBytes;
    int offset[2] = {0,0};
    int result;
    int progress;
    int page[2] = {0,0};
    char* imgBuffer = new char[MAX_SCAN_LENGTH];

    Setting* setting = &parameters.setting;
    duplex = setting->ADFMode;

    FILE* file[2];
    bool fileIsOpened[2] = {false ,false};

    m_cancel = false;
    while (!m_cancel){
        if (scannerApi->getInfo(sc_infodata)){
            usleep(100 * 1000);
            continue;
        }
        if(sc_infodata.CoverOpen){
            result = RETSCAN_COVER_OPEN;
            LOG_NOPARA("cover open.");
            break;
        }
        if(sc_infodata.PaperJam){
            if(sc_infodata.AdfSensor){
            //There is "scan jam" let scan flow finish for save image
            }else{
                LOG_NOPARA("paper jam.");
                result = RETSCAN_PAPER_JAM;
                break;
            }
        }
        if ((!(duplex & 1) || sc_infodata.EndScan[0])
                && (!(duplex & 2) || sc_infodata.EndScan[1])){
            LOG_NOPARA("end scan.");
            break;
        }

        for (dup = 0; dup < 2; dup++){
            if (duplex & (1 << dup) && sc_infodata.ValidPageSize[dup]){
                if(!fileIsOpened[dup]){
                    file[dup] = fopen(pPlatformApp->getTempFilename(dup) ,"wb");
                    fileIsOpened[dup] = true;
//                    LOG_PARA("side %c page %d begin." ,dup?'B':'A' ,page[dup]);
                }
                toReadBytes = sc_infodata.ValidPageSize[dup];
                LOG_PARA("receive %c side %d bytes:" ,dup ? 'B' :'A' ,toReadBytes);
                readTotalBytes[dup] = 0;
                while (readTotalBytes[dup] < toReadBytes) {
                    if(m_cancel){
                        goto EXIT;
//                        break;
                    }
                    result = scannerApi->readScan(dup ,&readBytes ,imgBuffer
                                                 ,toReadBytes - readTotalBytes[dup]);
                    if(!result){
                        readTotalBytes[dup] += readBytes;
                        offset[dup] += readBytes;
                        if(readBytes && fileIsOpened[dup])
                            fwrite(imgBuffer ,1 ,readBytes ,file[dup]);
                        LOG_PARA("receive %c side %d bytes:%d ----- read once:%d"
                                 ,dup ? 'B' :'A' ,toReadBytes ,readTotalBytes[dup] ,readBytes);
                    }else{
                        goto EXIT;
//                        break;
                    }
                    usleep(100 * 1000);
                }
                if(offset[dup] > progress)
                    progress = offset[dup];

                if(sc_infodata.EndPage[dup]
                        &&  readTotalBytes[dup] >= sc_infodata.ValidPageSize[dup]
                        ){
                    LOG_PARA("end page %c." ,dup ?'B' :'A');
                    offset[dup] = 0;
                    if(fileIsOpened[dup]){
                        fclose(file[dup]);
                        fileIsOpened[dup] = false;
                        pPlatformApp->saveScanImage(setting ,dup ,page[dup]++ ,sc_infodata.ImageHeight[dup]);
                    }
                }
            }
        }
        pPlatformApp->updateProgress(progress * 1.0 / (parameters.bytesPerLine * parameters.nColPixelNumOrig));
    }
    EXIT:
    for(int i = 0 ;i < 2 ;i++){
        if(fileIsOpened[i]){
            fclose(file[i]);
        }
    }
    delete [] imgBuffer;
    return result;
}

int Scanner::ADFScan()
{
    int result;
    result = scannerApi->jobCreate(CODE_ADF);
    if(result){
        int errorcode = RETSCAN_CREATE_JOB_FAIL;
        switch (result) {
        case ADF_NOT_READY_ERR:            errorcode = RETSCAN_ADF_NOT_READY;            break;
        case DOC_NOT_READY_ERR:            errorcode = RETSCAN_PAPER_NOT_READY;            break;
        case HOME_NOT_READY_ERR:            errorcode = RETSCAN_HOME_NOT_READY;            break;
        case SCAN_JAM_ERR:            errorcode = RETSCAN_PAPER_JAM;            break;
        case COVER_OPEN_ERR:            errorcode = RETSCAN_COVER_OPEN;            break;
        default:break;
        }
        result = errorcode;
        LOG_PARA("job create err:%d" ,result);
    }else{
        result = _scan();
        scannerApi->jobEnd();
        LOG_PARA("scan err:%d" ,result);
    }
    return result;
}

int Scanner::_scan()
{
    int result;
    Setting* setting = getSetting();
    SC_PAR_DATA_T para;
    getScanParameters(*setting ,&para);
    result = scannerApi->setParameters(para);
    if(result){
        return RETSCAN_ERRORPARAMETER;
    }

    result = scannerApi->startScan();
    if(result){
        return RETSCAN_ERROR;
    }

    //receive data loop
    result = receiveData();
    if(m_cancel){
        scannerApi->cancelScan();
        waitJobFinish(0);
        return RETSCAN_CANCEL;
    }

    scannerApi->stopScan();
    waitJobFinish(0);
    return result;
}

#define JOB_WAIT_TIMEOUT  5000
int Scanner::waitJobFinish(int wait_motor_stop)
{
    SC_INFO_DATA_T sc_infodata;
    for(int i = 0 ;i < 100 ;i++){
        if (scannerApi->getInfo(sc_infodata))
            break;
        if (!(sc_infodata.JobState & 1) && (!wait_motor_stop || !sc_infodata.MotorMove))
            return 0;
        sleep(100);
    }
    return -1;
}

Scanner::Setting* Scanner::getSetting()
{
    return &parameters.setting;
}

static int GetByteNumPerLineWidthPad(int scanMode, int nPixels)
{
    int cbPerLine = 0;

    switch (scanMode)
    {
    case _SCANMODE_1BIT_BLACKWHITE:
        cbPerLine = (nPixels + 7) / 8;
        break;
    case _SCANMODE_8BIT_GRAYSCALE:
        cbPerLine = nPixels;
        break;
    case _SCANMODE_24BIT_COLOR:
        cbPerLine = 3 * nPixels;
        break;
    default:
        cbPerLine = 0;
    }

    return cbPerLine;
}

void Scanner::calculateParameters(const Setting& setting)
{
    int width ,height;
    switch (setting.scan_doc_size) {
    case SETTING_DOC_SIZE_A4:
        width = 2480;
        height = 3512;
//        parameters.x = (2592 - 2480) / 2;
        break;
    case SETTING_DOC_SIZE_LT:
        width = 2552;
        height = 3296;
//        parameters.x = (2592 - 2552) / 2;
        break;
    case SETTING_DOC_SIZE_FULL:
    default:
        width = 2592 ;
        height = 118*300 ;
//        parameters.x = 0;
        break;
    }
    parameters.x = ((2592 - width) / 2) * setting.dpi_x / 300;
    parameters.y = 0;
    parameters.nLinePixelNumOrig = width * setting.dpi_x / 300;//1000;
    parameters.nLinePixelNumOrig -= parameters.nLinePixelNumOrig % 8;
    parameters.nColPixelNumOrig = height * setting.dpi_y / 300;//1000;
    parameters.nColPixelNumOrig -= parameters.nColPixelNumOrig % 8;
    parameters.bytesPerLine = GetByteNumPerLineWidthPad(setting.BitsPerPixel, parameters.nLinePixelNumOrig);

}

void Scanner::getScanParameters(const Setting& setting ,SC_PAR_DATA_T* para)
{
    calculateParameters(setting);

    memset(para ,0 ,sizeof(*para));
//    para			    = { SCAN_SOURCE, SCAN_ACQUIRE, SCAN_OPTION, SCAN_DUPLEX, SCAN_PAGE,
//                            { IMG_FORMAT, IMG_OPTION, IMG_BIT, IMG_MONO,
//                            { IMG_DPI_X, IMG_DPI_Y },{ IMG_ORG_X, IMG_ORG_Y }, IMG_WIDTH, IMG_HEIGHT },
//                                //{{MTR_DRIV_TAR, MTR_STAT_MEC, MTR_DIRECT, MTR_MICRO_STEP, MTR_CURRENT, MTR_SPEED, MTR_ACC_STEP, 0},
//                                //{MTR_DRIV_TAR, MTR_STAT_MEC, MTR_DIRECT, MTR_MICRO_STEP, MTR_CURRENT, MTR_SPEED, MTR_ACC_STEP, 0}}
//                            { { 0 },
//                            { 0 } }
//                            };

    para->acquire = ((setting.MultiFeed ? 1 : 0) * ACQ_Ultra_Sonic) | ((setting.AutoCrop ? 1 : 0) * ACQ_CROP_DESKEW) | ACQ_PICK_SS;

    if(SETTING_SCAN_ADF == setting.source){
        para->source = CODE_ADF;
        para->duplex = setting.ADFMode;
    }else{
        para->source = CODE_FLB;
        para->duplex = SCAN_A_SIDE;
    }
    para->page = setting.onepage ? 1 : 0;
    para->img.format = SETTING_IMG_FORMAT_JPG == setting.format ?CODE_JPG :CODE_RAW;
    para->img.bit = setting.BitsPerPixel;
    para->img.dpi.x = setting.dpi_x;
    para->img.dpi.y = setting.dpi_y;
    para->img.org.x = parameters.x;
    para->img.org.y = parameters.y;

    para->img.width = parameters.nLinePixelNumOrig;
    para->img.height = parameters.nColPixelNumOrig;
    para->img.mono = setting.BitsPerPixel == SETTING_IMG_24_BIT ? IMG_COLOR : IMG_3CH_TRUE_MONO;

    if (para->img.format == CODE_JPG) {
        para->img.option = IMG_OPT_JPG_FMT444;
    }

    //Advanced
    if (para->acquire & ACQ_SET_MTR) {

            /*ADF scan*/
        para->mtr[1].pick_ss_step = 600;//GetPrivateProfileInt("PICK_SS_STEP", "STEP", 0, IniFile);

            //par->mtr[0].drive_target = CMT_PH;
            //par->mtr[0].state_mechine = SCAN_STATE_MECHINE;
            //par->mtr[1].drive_target = BMT_PH;
            //par->mtr[1].state_mechine = STATE_MECHINE_1;
        if (para->img.bit < 24) {
            /*Mono scan*/
            para->mtr[1].speed_pps = 3456;// GetPrivateProfileInt("ADFgray", "PICK_PPS", 0, IniFile);
            para->mtr[1].direction = 1;// GetPrivateProfileInt("ADFgray", "PICK_DIR", 0, IniFile);
            para->mtr[1].micro_step = 2;// GetPrivateProfileInt("ADFgray", "PICK_MS", 0, IniFile);
            para->mtr[1].currentLV = 4;// GetPrivateProfileInt("ADFgray", "PICK_CLV", 0, IniFile);

            para->mtr[0].speed_pps = 4577;// GetPrivateProfileInt("ADFgray", "FEED_PPS", 0, IniFile);
            para->mtr[0].direction = 0;// GetPrivateProfileInt("ADFgray", "FEED_DIR", 0, IniFile);
            para->mtr[0].micro_step = 4;// GetPrivateProfileInt("ADFgray", "FEED_MS", 0, IniFile);
            para->mtr[0].currentLV = 4;// GetPrivateProfileInt("ADFgray", "FEED_CLV", 0, IniFile);
        }
        else {
            /*Color scan*/
            if ((para->img.dpi.x == 300) && (para->img.dpi.y == 300)) {
                /*300x300DPI scan*/
                para->mtr[1].speed_pps = 2602;// GetPrivateProfileInt("ADF300x300color", "PICK_PPS", 0, IniFile);
                para->mtr[1].direction = 1;// GetPrivateProfileInt("ADF300x300color", "PICK_DIR", 0, IniFile);
                para->mtr[1].micro_step = 2;// GetPrivateProfileInt("ADF300x300color", "PICK_MS", 0, IniFile);
                para->mtr[1].currentLV = 4;// GetPrivateProfileInt("ADF300x300color", "PICK_CLV", 0, IniFile);

                para->mtr[0].speed_pps = 350;// GetPrivateProfileInt("ADF300x300color", "FEED_PPS", 0, IniFile);
                para->mtr[0].direction = 0;// GetPrivateProfileInt("ADF300x300color", "FEED_DIR", 0, IniFile);
                para->mtr[0].micro_step = 4;// GetPrivateProfileInt("ADF300x300color", "FEED_MS", 0, IniFile);
                para->mtr[0].currentLV = 4;// GetPrivateProfileInt("ADF300x300color", "FEED_CLV", 0, IniFile);
            }
            else if ((para->img.dpi.x == 300) && (para->img.dpi.y == 600)) {
                /*300x600DPI scan*/
                para->mtr[1].speed_pps = 2317;// GetPrivateProfileInt("ADF300x600color", "PICK_PPS", 0, IniFile);
                para->mtr[1].direction = 1;// GetPrivateProfileInt("ADF300x600color", "PICK_DIR", 0, IniFile);
                para->mtr[1].micro_step = 4;// GetPrivateProfileInt("ADF300x600color", "PICK_MS", 0, IniFile);
                para->mtr[1].currentLV = 4;// GetPrivateProfileInt("ADF300x600color", "PICK_CLV", 0, IniFile);

                para->mtr[0].speed_pps = 2612;// GetPrivateProfileInt("ADF300x600color", "FEED_PPS", 0, IniFile);
                para->mtr[0].direction = 0;// GetPrivateProfileInt("ADF300x600color", "FEED_DIR", 0, IniFile);
                para->mtr[0].micro_step = 4;// GetPrivateProfileInt("ADF300x600color", "FEED_MS", 0, IniFile);
                para->mtr[0].currentLV = 4;// GetPrivateProfileInt("ADF300x600color", "FEED_CLV", 0, IniFile);
            }
            else {
                /*600DPI scan*/
                para->mtr[1].speed_pps = 1293;// GetPrivateProfileInt("ADF600x600color", "PICK_PPS", 0, IniFile);
                para->mtr[1].direction = 1;// GetPrivateProfileInt("ADF600x600color", "PICK_DIR", 0, IniFile);
                para->mtr[1].micro_step = 4;// GetPrivateProfileInt("ADF600x600color", "PICK_MS", 0, IniFile);
                para->mtr[1].currentLV = 4;// GetPrivateProfileInt("ADF600x600color", "PICK_CLV", 0, IniFile);

                para->mtr[0].speed_pps = 1557;// GetPrivateProfileInt("ADF600x600color", "FEED_PPS", 0, IniFile);
                para->mtr[0].direction = 0;// GetPrivateProfileInt("ADF600x600color", "FEED_DIR", 0, IniFile);
                para->mtr[0].micro_step = 4;// GetPrivateProfileInt("ADF600x600color", "FEED_MS", 0, IniFile);
                para->mtr[0].currentLV = 4;// GetPrivateProfileInt("ADF600x600color", "FEED_CLV", 0, IniFile);
            }

        }
//        switch (para->mtr[0].currentLV) {
//        case 1:            para->mtr[0].currentLV = 0;            break;
//        case 2:            para->mtr[0].currentLV = 2;            break;
//        case 3:            para->mtr[0].currentLV = 1;            break;
//        case 4:            para->mtr[0].currentLV = 3;            break;
//        default:
//            break;
//        }
//        switch (para->mtr[1].currentLV) {
//        case 1:            para->mtr[1].currentLV = 0;            break;
//        case 2:            para->mtr[1].currentLV = 2;            break;
//        case 3:            para->mtr[1].currentLV = 1;            break;
//        case 4:            para->mtr[1].currentLV = 3;            break;
//        default:
//            break;
//        }
    }
}
