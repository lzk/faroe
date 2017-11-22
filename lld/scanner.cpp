#include "scanner.h"
#include <unistd.h>
#include "platformapp.h"
#include "scannerapi.h"
using namespace JK;

#define _SCANMODE_1BIT_BLACKWHITE 1
#define _SCANMODE_8BIT_GRAYSCALE  8
#define _SCANMODE_24BIT_COLOR     24

const Scanner::Setting defaultSetting = {
    .BitsPerPixel = IMG_BIT,
    .resolution = IMG_DPI_X,
    .width=IMG_WIDTH,
    .height=IMG_HEIGHT,
    .x=0,
    .y=0,
    .contrast=50,
    .brightness=50,
    .ADFMode=SCAN_DUPLEX,
    .MultiFeed=true,
    .AutoCrop=true,
    .onepage=SCAN_PAGE,
    .source=SCAN_SOURCE,
    .format=IMG_FORMAT,
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
    int readBytes ,readTotalBytes ,toReadBytes;
    int offset[2] = {0,0};
    int result;
    int progress = 0;

    char fileName[256];
    Setting* setting = &parameters.setting;
    char suffix[5];
    char colorType = 'C';
    switch (setting->BitsPerPixel) {
    case _SCANMODE_1BIT_BLACKWHITE:        colorType = 'B';        break;
    case _SCANMODE_8BIT_GRAYSCALE:        colorType = 'G';        break;
    case _SCANMODE_24BIT_COLOR:        colorType = 'C';        break;
    default:
        break;
    }
    if(setting->format == IMG_FMT_JPG){
        strcpy(suffix ,".jpg");
    }else if(setting->format == IMG_FMT_BMP){
        strcpy(suffix ,".bmp");
    }else if(setting->format == IMG_FMT_RAW){
        strcpy(suffix ,".raw");
    }else if(setting->format == IMG_FMT_TIF){
        strcpy(suffix ,".tif");
    }
    duplex = setting->ADFMode;
    m_cancel = false;
    int page[2] = {0,0};
    while (!m_cancel) {
        if (scannerApi->getInfo(sc_infodata)){
            usleep(100 * 1000);
            continue;
        }
        if(sc_infodata.CoverOpen){

            break;
        }
        if(sc_infodata.PaperJam){
            if(sc_infodata.AdfSensor){

            }else{
                break;
            }
        }
        if ((!(duplex & 1) || sc_infodata.EndScan[0])
                && (!(duplex & 2) || sc_infodata.EndScan[1]))
            break;


        for (dup = 0; dup < 2; dup++){
            if (duplex & (1 << dup)){
                readTotalBytes = 0;
                toReadBytes = sc_infodata.ValidPageSize[dup];
                if(toReadBytes + offset[dup] > parameters.imgBufferSize)
                    toReadBytes = parameters.imgBufferSize - offset[dup];
                while (readTotalBytes < toReadBytes) {
                    if(m_cancel)
                        break;
                    result = scannerApi->readScan(dup ,&readBytes ,(char*)parameters.imgBuffer + offset[dup]
                                                 ,toReadBytes - readTotalBytes);
                    if(!result){
                        readTotalBytes += readBytes;
                        offset[dup] += readBytes;
                    }else{
                        break;
                    }
                    usleep(100 * 1000);
                }
                if(m_cancel)
                    break;

                if(offset[dup] > progress)
                    progress = offset[dup];
                if(offset[dup] == parameters.imgBufferSize &&  sc_infodata.EndPage[dup]){
                    offset[dup] = 0;
                    sprintf(fileName, "_%c%d_%c%02d.%s"
                            , colorType
                            , parameters.setting.resolution,
                            dup == 0 ?'A':'B', page[dup]++, suffix);
                    pPlatformApp->saveImage(fileName ,parameters.imgBuffer ,setting->width ,setting->height ,setting->BitsPerPixel);
                }
                //any bytes?
//                toReadBytes = sc_infodata.ValidPageSize[dup] - toReadBytes;
            }
        }
        if(m_cancel){
            break;
        }
        pPlatformApp->updateProgress(progress*1.0 / parameters.imgBufferSize);
    }
    return m_cancel || result;
}

int Scanner::ADFScan()
{
    int result;
    result = scannerApi->jobCreate();
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
    }else{
        result = _scan();
        scannerApi->jobEnd();
    }
    if(parameters.imgBuffer){
        delete parameters.imgBuffer;
        parameters.imgBuffer = NULL;
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
    if(result){
        if(m_cancel){
            scannerApi->cancelScan();
            return RETSCAN_CANCEL;
        }
        return RETSCAN_ERROR;
    }

    scannerApi->stopScan();
    waitJobFinish(0);
    return RETSCAN_OK;
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
    parameters.nLinePixelNumOrig = setting.width * setting.resolution / 300;//1000;
    parameters.nLinePixelNumOrig = parameters.nLinePixelNumOrig - parameters.nLinePixelNumOrig % 8;
    parameters.nColPixelNumOrig = setting.height * setting.resolution / 300;//1000;
    parameters.bytesPerLine = GetByteNumPerLineWidthPad(setting.BitsPerPixel, parameters.nLinePixelNumOrig);
    parameters.imgBufferSize = parameters.bytesPerLine * setting.height;

    if(parameters.imgBuffer != NULL)
        delete parameters.imgBuffer;
    parameters.imgBuffer = new unsigned char[parameters.imgBufferSize];
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

    para->source =IMG_FMT_ADF;
    para->duplex = setting.ADFMode;
    para->page = setting.onepage ? 1 : 0;
    para->img.format = IMG_FMT_JPG;
    para->img.bit = setting.BitsPerPixel;
    para->img.dpi.x = setting.resolution;
    para->img.dpi.y = setting.resolution;
    para->img.org.x = setting.x;
    para->img.org.y = setting.y;

    para->img.width = parameters.nLinePixelNumOrig;
    para->img.height = parameters.nColPixelNumOrig;
    para->img.mono = setting.BitsPerPixel == IMG_24_BIT ? IMG_COLOR : IMG_3CH_TRUE_MONO;

    if (para->img.format == IMG_FMT_JPG) {
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
    }
}
