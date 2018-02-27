#ifndef SCANNER_H
#define SCANNER_H
namespace JK {

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

class DeviceIO;
class PlatformApp;
class ScannerAPI;
struct SC_PAR_DATA_STRUCT;
struct SC_INFO_DATA_STRUCT;
class Scanner
{
public:
    typedef struct{
        int BitsPerPixel;
        int dpi_x;
        int dpi_y;
        int scan_doc_size;
        int contrast;
        int brightness;
        int ADFMode;
        bool MultiFeed;
        bool AutoCrop;
        bool onepage;

        int source;
        int format;
        bool bColorDetect;
        bool bSkipBlankPage;
        int gammaValue;
        int type;
    } Setting;


    typedef struct{
        Setting setting;
//        int width;
//        int height;
        int x;
        int y;
        int nColPixelNumOrig;
        int nLinePixelNumOrig;
        int bytesPerLine;
    }
        Parameters;

    typedef struct{
        int dup;
        int page[2];
        int lines;
        bool isColor;
        bool isBlank;
        int dpi_x;
        int dpi_y;
    }
        Para_Extra;
public:
    Scanner();
    ~Scanner();
    void install(DeviceIO* dio ,PlatformApp* platformApp);

//    int open(const char* );
    int open();
    int close();
    int ADFScan(void* setting);
    void cancel();

    Setting* getSetting();
private:
    DeviceIO* deviceIO;
    PlatformApp* pPlatformApp;
    ScannerAPI* scannerApi;
    bool m_cancel;
    Parameters parameters;
    int waitJobFinish(int wait_motor_stop);
    int receiveData();
    void getScanParameters(const Setting& setting ,struct SC_PAR_DATA_STRUCT* para);
    void calculateParameters(const Setting& setting);
    int _scan(Setting* setting);
    int _ADFScan(Setting* setting);
    int checkStatus(int stage ,struct SC_INFO_DATA_STRUCT* sc_infodata);
    void getGammaTable(float gamma ,unsigned int*);
};

}
#endif // SCANNER_H
