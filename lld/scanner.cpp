#include "scanner.h"
#include <unistd.h>
#include <string.h>
#include "platformapp.h"
#include "scannerapi.h"
#include "../platform/log.h"
#include "device.h"
#include "deviceio.h"
#include "../platform/devicestruct.h"
#include "../platform/log.h"
using namespace JK;

#define _SCANMODE_1BIT_BLACKWHITE 1
#define _SCANMODE_8BIT_GRAYSCALE  8
#define _SCANMODE_24BIT_COLOR     24

#define START_STAGE					0x1
#define SCANNING_STAGE				0x2
#define PUSH_TRANSFER_STAGE		0x3

const Scanner::Setting defaultSetting = {
    .BitsPerPixel = IMG_BIT,
    .dpi_x=200,
    .dpi_y=200,
    .scan_doc_size=SETTING_papersize_auto,
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
    deviceIO = dio;
    scannerApi->install(dio);
    pPlatformApp = platformApp;
}

int Scanner::open()
{
    if(deviceIO->type() == DeviceIO::Type_usb)
        return deviceIO->open();

    int result = 0;
//        result = deviceIO->read(&status ,1);
    result = ((NetDeviceIO*)deviceIO)->openPort(23011);
    if(!result){
        result = scannerApi->lock();
        if(!result && !m_cancel){
            result = deviceIO->close();
            if(!result)
                result = deviceIO->open();
        }else{
//            scannerApi->unlock();
            deviceIO->close();
        }
    }
    return result;
}

int Scanner::close()
{
    int result = deviceIO->close();
    if(deviceIO->type() == DeviceIO::Type_usb)
        return result;
    if(!result){
        result = ((NetDeviceIO*)deviceIO)->openPort(23011);
        if(!result){
            result = scannerApi->unlock();
            if(!result)
                result = deviceIO->close();
        }
    }
    return result;
}

void Scanner::cancel(bool iscancel)
{
    m_cancel = iscancel;
}

int Scanner::receiveData()
{
    SC_INFO_DATA_T sc_infodata;
    int duplex;
    int readBytes ,readTotalBytes[2] ,toReadBytes;
    int offset[2] = {0,0};
    int result;
    int progress;
    char* imgBuffer = new char[MAX_SCAN_LENGTH];

    Setting* setting = &parameters.setting;
    duplex = setting->ADFMode;

    FILE* file[2];
    bool fileIsOpened[2] = {false ,false};

    Para_Extra para;
    memset(&para ,0 ,sizeof(para));

    while (!m_cancel){
        result = checkStatus(SCANNING_STAGE ,&sc_infodata);
//        if(result == RETSCAN_GETINFO_FAIL){
//            usleep(100 * 1000);
//            continue;
//        }
        if(result)
            break;
        if ((!(duplex & 1) || sc_infodata.ImgStatus[0].EndScan)
                && (!(duplex & 2) || sc_infodata.ImgStatus[1].EndScan)){
            LOG_NOPARA("end scan.");
            break;
        }

        for (para.dup = 0; para.dup < 2; para.dup++){
            if (duplex & (1 << para.dup) && sc_infodata.ValidPageSize[para.dup]){
                if(!fileIsOpened[para.dup]){
                    file[para.dup] = fopen(pPlatformApp->getTempFilename(para.dup) ,"wb");
                    fileIsOpened[para.dup] = true;
//                    LOG_PARA("side %c page %d begin." ,para.dup?'B':'A' ,para.page[para.dup]);
                    if(duplex == SETTING_SCAN_AB_SIDE){
                        if(para.dup == 0)
                            pPlatformApp->updateProgress(DeviceStruct::ScanningProgress_Upload ,para.page[para.dup]);
                    }else{
                        pPlatformApp->updateProgress(DeviceStruct::ScanningProgress_Upload ,para.page[para.dup]);
                    }
                }
                toReadBytes = sc_infodata.ValidPageSize[para.dup];
                LOG_PARA("receive %c side %d bytes:" ,para.dup ? 'B' :'A' ,toReadBytes);
                readTotalBytes[para.dup] = 0;
                while (readTotalBytes[para.dup] < toReadBytes) {
                    if(m_cancel){
                        goto EXIT;
//                        break;
                    }
                    result = scannerApi->readScan(para.dup ,&readBytes ,imgBuffer
                                                 ,toReadBytes - readTotalBytes[para.dup]);
                    if(!result){
                        readTotalBytes[para.dup] += readBytes;
                        offset[para.dup] += readBytes;
                        if(readBytes && fileIsOpened[para.dup])
                            fwrite(imgBuffer ,1 ,readBytes ,file[para.dup]);
                        LOG_PARA("receive %c side %d bytes:%d ----- read once:%d"
                                 ,para.dup ? 'B' :'A' ,toReadBytes ,readTotalBytes[para.dup] ,readBytes);
                    }else{
                        goto EXIT;
//                        break;
                    }
                    usleep(100 * 1000);
                }
                if(offset[para.dup] > progress)
                    progress = offset[para.dup];

                if(sc_infodata.ImgStatus[para.dup].EndPage
                        &&  readTotalBytes[para.dup] >= sc_infodata.ValidPageSize[para.dup]
                        ){
                    LOG_PARA("end page %c." ,para.dup ?'B' :'A');
                    offset[para.dup] = 0;
                    if(fileIsOpened[para.dup]){
                        fclose(file[para.dup]);
                        fileIsOpened[para.dup] = false;

                        para.dpi_x = setting->dpi_x;
                        para.dpi_y = setting->dpi_y;
                        if(setting->bColorDetect)
                            para.isColor = sc_infodata.ImgStatus[para.dup].IsColor;
                        else
                            para.isColor = setting->BitsPerPixel >= 24;
                        para.isBlank = sc_infodata.ImgStatus[para.dup].IsBlank;

                        para.lines = sc_infodata.ImageHeight[para.dup];
                        pPlatformApp->saveScanImage(setting ,&para);
                        if(duplex == SETTING_SCAN_AB_SIDE){
                            if(para.dup == 0)
                                pPlatformApp->updateProgress(DeviceStruct::ScanningProgress_Completed ,para.page[para.dup]);
                        }else{
                            pPlatformApp->updateProgress(DeviceStruct::ScanningProgress_Completed ,para.page[para.dup]);
                        }
                        para.page[para.dup]++;
                    }
                }
            }
        }
    }
    EXIT:
    if(m_cancel){
        result = RETSCAN_CANCEL;
    }
    for(int i = 0 ;i < 2 ;i++){
        if(fileIsOpened[i]){
            fclose(file[i]);
        }
    }
    delete [] imgBuffer;
    return result;
}

int Scanner::_ADFScan(Setting* setting)
{
    int result;
    result = scannerApi->jobCreate(JOB_ADF);
    if(result){
        int errorcode = RETSCAN_CREATE_JOB_FAIL;
//        switch (result) {
//        case ADF_NOT_READY_ERR:            errorcode = RETSCAN_ADF_NOT_READY;            break;
//        case DOC_NOT_READY_ERR:            errorcode = RETSCAN_PAPER_NOT_READY;            break;
//        case HOME_NOT_READY_ERR:            errorcode = RETSCAN_HOME_NOT_READY;            break;
//        case SCAN_JAM_ERR:            errorcode = RETSCAN_PAPER_JAM;            break;
//        case COVER_OPEN_ERR:            errorcode = RETSCAN_COVER_OPEN;            break;
//        default:break;
//        }
        result = errorcode;
//        LOG_PARA("job create err:%d" ,result);
    }else{
        result = _scan(setting);
        scannerApi->jobEnd();
        LOG_PARA("scan err:%d" ,result);
    }
    return result;
}

#include <math.h>
bool isLittleEndian()
{
    int i = 1;
    return (((char*)(&i))[0] == 1);
}
void Scanner::getGammaTable(float gamma ,unsigned int* GLGamma)
{
    bool littleEndian = isLittleEndian();
    qDebug()<<"gamma:"<<gamma <<"litten?"<<littleEndian;
    if(gamma < 0.1)
        gamma = 0.1;
//    unsigned int Red[65536];
//    unsigned int Green[65536];
//    unsigned int Blue[65536];
    unsigned int* Red = new unsigned int [65536];
//    unsigned int* Green =  new unsigned int [65536];
//    unsigned int* Blue =  new unsigned int [65536];
    unsigned int *pbyRed = Red;
//    unsigned int *pbyGreen = Green;
//    unsigned int *pbyBlue = Blue;
    unsigned int Temp;
    int i;
    for (i = 0; i<65536; i++)
    {
        Temp = (long)ceil(65535 * pow((double)(i) / 65535, 1.0 / gamma));
        if (Temp>65535){
            Temp = 65535;
        }else{
            if (Temp<=0){
                Temp = 0;
            }
        }
        Red[i] = Temp;
//        Green[i] = Temp;
//        Blue[i] = Temp;
    }
    for (i = 0; i<256; i++)
    {
        if (i<255) {
            if(littleEndian){
                GLGamma[i] = (pbyRed[i*256] & 0xffff0000)
                        | ((pbyRed[(i+1)*256] & 0xffff0000) >> 16);
//                GLGamma[i + 256] = (pbyGreen[i*256] & 0xffff0000)
//                        | ((pbyGreen[(i+1)*256] & 0xffff0000) >> 16);
//                GLGamma[i + 512] = (pbyBlue[i*256] & 0xffff0000)
//                        | ((pbyBlue[(i+1)*256] & 0xffff0000) >> 16);
            }else{
                GLGamma[i] = (pbyRed[i*256] & 0x0000ffff) | (pbyRed[(i+1)*256] & 0x0000ffff << 16);
//                GLGamma[i + 256] = (pbyGreen[i*256] & 0x0000ffff) | (pbyGreen[(i+1)*256] & 0x0000ffff << 16);
//                GLGamma[i + 256 * 2] = (pbyBlue[i*256] & 0x0000ffff) | (pbyBlue[(i+1)*256] & 0x0000ffff << 16);
            }
        }
        else {
            if(littleEndian){
                GLGamma[i] = (pbyRed[i*256] & 0xffff0000) | 0x0000ffff;
//                GLGamma[i + 256] = (pbyGreen[i*256] & 0xffff0000) | 0x0000ffff;
//                GLGamma[i + 512] = (pbyBlue[i*256] & 0xffff0000) | 0x0000ffff;
            }else{
                GLGamma[i] = (pbyRed[i*256] & 0x0000ffff) | 0xffff0000;
//                GLGamma[i + 256] = (pbyGreen[i*256] & 0x0000ffff) | 0xffff0000;
//                GLGamma[i + 256 * 2] = (pbyBlue[i*256] & 0x0000ffff) | 0xffff0000;
            }
        }
        GLGamma[i + 256] = GLGamma[i];
        GLGamma[i + 512] = GLGamma[i];
    }
    delete [] Red;
//    delete [] Green;
//    delete [] Blue;
}

int Scanner::_scan(Setting* setting)
{
    if(m_cancel){
        return RETSCAN_CANCEL;
    }
    int result;
    SC_PAR_DATA_T para;
    getScanParameters(*setting ,&para);
    result = scannerApi->setParameters(para);
    if(result){
        return RETSCAN_ERRORPARAMETER;
    }

    if(m_cancel){
        return RETSCAN_CANCEL;
    }
    unsigned int gGammaData[768];
    getGammaTable(setting->gammaValue / 10.0 ,gGammaData);
    result = scannerApi->setGamma(gGammaData ,768);
    if(result){
        return RETSCAN_ERRORPARAMETER;
    }

    pPlatformApp->updateProgress(DeviceStruct::ScanningProgress_Start ,0);

    if(m_cancel){
        return RETSCAN_CANCEL;
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

int Scanner::checkStatus(int stage ,SC_INFO_DATA_T* sc_infodata)
{
    int result = 0;
    result = scannerApi->getInfo(*sc_infodata);
    if (!result){
        switch (stage) {
        case START_STAGE:
            if(sc_infodata->JobID)
                result = RETSCAN_JOB_GOGING;
            break;
        case SCANNING_STAGE:
            if(!sc_infodata->JobID)
                result = RETSCAN_JOB_MISSING;
            break;
        case PUSH_TRANSFER_STAGE:
            if(sc_infodata->ErrorStatus.cover_open_err)
                result = RETSCAN_COVER_OPEN;
            break;
        default:
            break;
        }
        if(sc_infodata->ErrorStatus.scan_jam_err)
            result = RETSCAN_PAPER_JAM;
        if(sc_infodata->ErrorStatus.scan_canceled_err)
            result = RETSCAN_CANCEL;
        if(sc_infodata->ErrorStatus.scan_timeout_err)
            result = RETSCAN_TIME_OUT;
        if(sc_infodata->ErrorStatus.multi_feed_err)
            result = RETSCAN_ULTRA_SONIC;
        if(sc_infodata->ErrorStatus.usb_transfer_err)
            result = RETSCAN_USB_TRANSFERERROR;
        if(sc_infodata->ErrorStatus.wifi_transfer_err)
            result = RETSCAN_WIFI_TRANSFERERROR;
        if(stage == START_STAGE){
            if(sc_infodata->SensorStatus.adf_document_sensor)
                result = RETSCAN_ADFDOC_NOT_READY;
            if(sc_infodata->SensorStatus.adf_paper_sensor)
                result = RETSCAN_ADFPATH_NOT_READY;
            if(sc_infodata->SensorStatus.cover_sensor)
                result = RETSCAN_ADFCOVER_NOT_READY;
        }
    }else{
        result = RETSCAN_GETINFO_FAIL;
    }
    return result;
}

#define JOB_WAIT_TIMEOUT  5000
int Scanner::waitJobFinish(int wait_motor_stop)
{
    return 0;
//    SC_INFO_DATA_T sc_infodata;
//    for(int i = 0 ;i < 100 ;i++){
//        if (scannerApi->getInfo(sc_infodata))
//            break;
//        if (!(sc_infodata.JobState & 1) && (!wait_motor_stop || !sc_infodata.MotorMove))
//            return 0;
//        sleep(100);
//    }
//    return -1;
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
    case SETTING_papersize_auto:
    case SETTING_papersize_autoNoMultiFeed:
        width = 8500;
        height = 15000;
        break;
    case SETTING_papersize_A4:
        width = 8268;
        height = 11693;
        break;
    case SETTING_papersize_A5:
        width = 5827;
        height = 8268;
        break;
    case SETTING_papersize_B5:
        width = 7165;
        height = 10118;
        break;
    case SETTING_papersize_A6:
        width = 4134;
        height = 5827;
        break;
    case SETTING_papersize_letter:
        width = 8500;
        height = 11000;
        break;
    case SETTING_papersize_legal:
        width = 8500;
        height = 14000;
        break;
    case SETTING_papersize_long:
        width = 8500;
        height = 118110;
        break;
    default:
        width = 8500;
        height = 15000;
        break;

//    case SETTING_DOC_SIZE_A4:
//        width = 2480;
//        height = 3512;
////        parameters.x = (2592 - 2480) / 2;
//        break;
//    case SETTING_DOC_SIZE_LT:
//        width = 2552;
//        height = 3296;
////        parameters.x = (2592 - 2552) / 2;
//        break;
//    case SETTING_DOC_SIZE_FULL:
//    default:
//        width = 2592 ;
//        height = 118*300 ;
////        parameters.x = 0;
//        break;
    }
//    parameters.x = ((2592 - width) / 2) * setting.dpi_x / 300;
//    parameters.y = 0;
    parameters.nLinePixelNumOrig = width * setting.dpi_x / 1000;
    parameters.nLinePixelNumOrig -= parameters.nLinePixelNumOrig % 8;
    parameters.x = (8.5 * setting.dpi_x - parameters.nLinePixelNumOrig) / 2;
    parameters.nColPixelNumOrig = height * setting.dpi_y / 1000;
    parameters.nColPixelNumOrig -= parameters.nColPixelNumOrig % 8;
    parameters.y = (14 * setting.dpi_y - parameters.nColPixelNumOrig) / 2;
    parameters.bytesPerLine = GetByteNumPerLineWidthPad(setting.BitsPerPixel, parameters.nLinePixelNumOrig);
    parameters.setting = setting;
}

void Scanner::getScanParameters(const Setting& setting ,SC_PAR_DATA_T* para)
{
    memset(para ,0 ,sizeof(*para));
//    para			    = { SCAN_SOURCE, SCAN_ACQUIRE, SCAN_OPTION, SCAN_DUPLEX, SCAN_PAGE,
//                            { IMG_FORMAT, IMG_OPTION, IMG_BIT, IMG_MONO,
//                            { IMG_DPI_X, IMG_DPI_Y },{ IMG_ORG_X, IMG_ORG_Y }, IMG_WIDTH, IMG_HEIGHT },
//                                //{{MTR_DRIV_TAR, MTR_STAT_MEC, MTR_DIRECT, MTR_MICRO_STEP, MTR_CURRENT, MTR_SPEED, MTR_ACC_STEP, 0},
//                                //{MTR_DRIV_TAR, MTR_STAT_MEC, MTR_DIRECT, MTR_MICRO_STEP, MTR_CURRENT, MTR_SPEED, MTR_ACC_STEP, 0}}
//                            { { 0 },
//                            { 0 } }
//                            };

    para->acquire = ((setting.MultiFeed ? 1 : 0) * ACQ_ULTRA_SONIC)
            | ((setting.AutoCrop ? 1 : 0) * ACQ_CROP_DESKEW)
            | ACQ_PICK_SS
            |(setting.bColorDetect ?1 :0) * ACQ_DETECT_COLOR
            |(setting.bSkipBlankPage ?1 :0) * ACQ_SKIP_BLANKPAGE;

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

enum ScannerCmd{
    ScannerCmd_adfScan,
    ScannerCmd_getPwerSupply,
    ScannerCmd_getDeviceSettings,
    ScannerCmd_setPowerSaveTime,
    ScannerCmd_clearRollerCount,
    ScannerCmd_clearACMCount,
};

int Scanner::doScannerJob(int cmd ,void* data)
{
    int result = 0;
    if(deviceIO->type() == DeviceIO::Type_net){
//        result = deviceIO->read(&status ,1);
        result = ((NetDeviceIO*)deviceIO)->openPort(23011);
        if(!result){
            char cmd[4] = { 'J','D','G','S' };
            char status[8] = { 0 };
            if(4 == deviceIO->write(cmd ,4)){
                if(8==deviceIO->read(status ,8)){
                    if (   status[0] == 'J'
                        && status[1] == 'D'
                        && status[2] == 'A'
                           && status[3] == 'T'
                        && status[4] == 0x00){
                        result = RETSCAN_OK;
                    }else{
                        result = RETSCAN_BUSY;
                    }
                }
            }
            deviceIO->close();
        }
    }
    if(result != RETSCAN_OK)
        return result;
    result = open();
    if(!result){
        if(m_cancel){
            result = RETSCAN_CANCEL;
        }else
            result = scannerApi->ready() ?RETSCAN_OK :RETSCAN_BUSY;
    }
    if(!result){
        if(m_cancel){
            result = RETSCAN_CANCEL;
        }else
            result = scanner_cmd(cmd ,data);
    }
    close();
    return result;
}

int Scanner::scanner_cmd(int cmd ,void* data)
{
    int result = DeviceStruct::ERR_invalid_data;
    switch (cmd) {
    case ScannerCmd_adfScan:
    {
        result = adfScan(data);
    }
        break;
    case ScannerCmd_getPwerSupply:
    {
        if(!data)
            return DeviceStruct::ERR_invalid_data;
        SC_POWER_INFO_T sc_powerData;
        result = scannerApi->getPowerSupply(sc_powerData);
        if(!result){
            int* mode = (int*) data;
            *mode = sc_powerData.mode;
        }
    }
        break;
    case ScannerCmd_getDeviceSettings:
    {
        if(!data)
            return DeviceStruct::ERR_invalid_data;
        struct_deviceSetting* pData = (struct_deviceSetting*)data;
        {
            int sleepTime;int offTime;
            int powermode;
            result = scannerApi->getPowerSaveTime(sleepTime ,offTime ,powermode);
            if(!result){
                pData->saveTime = sleepTime;
                pData->offTime = offTime;
            }else{
                return -1;
            }
        }
        {
            unsigned char dt[4];
            result = scannerApi->nvram_read(0x48 ,4 ,dt);
            if(!result){
                pData->rollerCount = dt[0]
                        |(dt[1] & 0xff00)
                        |(dt[2] & 0xff0000)
                        |(dt[3] & 0xff000000);
            }else{
                return -1;
            }
            result = scannerApi->nvram_read(0x4c ,4 ,dt);
            if(!result){
                pData->acmCount = dt[0]
                        |(dt[1] & 0xff00)
                        |(dt[2] & 0xff0000)
                        |(dt[3] & 0xff000000);
            }else{
                return -1;
            }
            result = scannerApi->nvram_read(0x00 ,4 ,dt);
            if(!result){
                pData->scanCount = dt[0]
                        |(dt[1] & 0xff00)
                        |(dt[2] & 0xff0000)
                        |(dt[3] & 0xff000000);
            }else{
                return -1;
            }
        }
        break;
    }
    case ScannerCmd_setPowerSaveTime:
    {
        if(!data)
            return DeviceStruct::ERR_invalid_data;
        struct_deviceSetting* pData = (struct_deviceSetting*)data;
        int sleepTime = pData->saveTime;
        int offTime = pData->offTime;
        int powermode;
        result = scannerApi->setPowerSaveTime(sleepTime ,offTime ,powermode);
        break;
    }
    case ScannerCmd_clearRollerCount:
    {
        unsigned char dt[4] = {0};
        result = scannerApi->nvram_write(0x48 ,4 ,dt);
        break;
    }
    case ScannerCmd_clearACMCount:
    {
        unsigned char dt[4] = {0};
        result = scannerApi->nvram_write(0x4c ,4 ,dt);
        break;
    }
    default:
        break;
    }
    return result;
}


int Scanner::getPowerSupply(void* data)
{
    return doScannerJob(ScannerCmd_getPwerSupply ,data);
}

int Scanner::getDeviceSettings(void* data)
{
    return doScannerJob(ScannerCmd_getDeviceSettings ,data);
}

int Scanner::setPowerSaveTime(void* data)
{
    return doScannerJob(ScannerCmd_setPowerSaveTime ,data);
}

int Scanner::clearRollerCount()
{
    return doScannerJob(ScannerCmd_clearRollerCount ,NULL);
}

int Scanner::clearACMCount()
{
    return doScannerJob(ScannerCmd_clearACMCount ,NULL);
}

int Scanner::adfScan(void *data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;
    Scanner::Setting* setting = (Scanner::Setting*)data;
    int result;
    {
        SC_INFO_DATA_T sc_infodata;
        result = checkStatus(START_STAGE ,&sc_infodata);
    }
    if(!result){
        if(m_cancel){
            return RETSCAN_CANCEL;
        }
        calculateParameters(*setting);
        SC_POWER_INFO_T sc_powerData;
        result = scannerApi->getPowerSupply(sc_powerData);
        switch (sc_powerData.mode) {
        case 2:
            if(setting->AutoCrop || setting->type > 0 ||parameters.nColPixelNumOrig > 14000)
                result = RETSCAN_ERROR_POWER1;
            break;
        case 3:
            if(setting->AutoCrop
                    || setting->type > 0 //mediaType
                    ||parameters.nColPixelNumOrig > 14000
                    ||setting->ADFMode == SETTING_SCAN_AB_SIDE
                    ||setting->MultiFeed
                    || deviceIO->type() == DeviceIO::Type_net)
                result = RETSCAN_ERROR_POWER2;
            break;
        default:
            break;
        }
    }
    if(!result)
        result = _ADFScan(setting);
    return result;
}

#if 1
int Scanner::ADFScan(void* data)
{
    return doScannerJob(ScannerCmd_adfScan ,data);
}
#else
int Scanner::ADFScan(void* data)
{
    if(!data)
        return DeviceStruct::ERR_invalid_data;

    Scanner::Setting* setting = (Scanner::Setting*)data;
    int result = 0;
    if(deviceIO->type() == DeviceIO::Type_net){
//        result = deviceIO->read(&status ,1);
        result = ((NetDeviceIO*)deviceIO)->openPort(23011);
        if(!result){
            char cmd[4] = { 'J','D','G','S' };
            char status[8] = { 0 };
            if(4 == deviceIO->write(cmd ,4)){
                if(8==deviceIO->read(status ,8)){
                    if (   status[0] == 'J'
                        && status[1] == 'D'
                        && status[2] == 'A'
                           && status[3] == 'T'
                        && status[4] == 0x00){
                        result = RETSCAN_OK;
                    }else{
                        result = RETSCAN_BUSY;
                    }
                }
            }
            deviceIO->close();
        }
    }
    if(result != RETSCAN_OK)
        return result;
    result = open();
    if(!result){
        result = scannerApi->ready() ?RETSCAN_OK :RETSCAN_BUSY;
    }
    if(!result){
        SC_INFO_DATA_T sc_infodata;
        result = checkStatus(START_STAGE ,&sc_infodata);
    }
    if(!result){
        calculateParameters(*setting);
        SC_POWER_INFO_T sc_powerData;
        result = scannerApi->getPowerSupply(sc_powerData);
        switch (sc_powerData.mode) {
        case 2:
            if(setting->AutoCrop || setting->type > 0 ||parameters.nColPixelNumOrig > 14000)
                result = RETSCAN_ERROR_POWER1;
            break;
        case 3:
            if(setting->AutoCrop
                    || setting->type > 0 //mediaType
                    ||parameters.nColPixelNumOrig > 14000
                    ||setting->ADFMode == SETTING_SCAN_AB_SIDE
                    ||setting->MultiFeed
                    || deviceIO->type() == DeviceIO::Type_net)
                result = RETSCAN_ERROR_POWER2;
            break;
        default:
            break;
        }
    }
    if(!result)
        result = _ADFScan(setting);
    close();
    return result;
}
#endif

extern DeviceIO* usbio;
int DoCalibration();
int Scanner::doCalibration()
{
    if(deviceIO->type() == DeviceIO::Type_net){
        return DeviceStruct::ERR_cmd_cannot_support;
    }
    usbio = deviceIO;
    return DoCalibration();
}
