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
    RETSCAN_ADF_NOT_READY = 15,
    RETSCAN_HOME_NOT_READY = 16,
};

class DeviceIO;
class PlatformApp;
class ScannerAPI;
struct SC_PAR_DATA_STRUCT;
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

public:
    Scanner();
    ~Scanner();
    void install(DeviceIO* dio ,PlatformApp* platformApp);
    int ADFScan();
    void cancel();

    Setting* getSetting();
private:
    PlatformApp* pPlatformApp;
    ScannerAPI* scannerApi;
    bool m_cancel;
    Parameters parameters;
    int waitJobFinish(int wait_motor_stop);
    int receiveData();
    Parameters getParameters();
    void getScanParameters(const Setting& setting ,struct SC_PAR_DATA_STRUCT* para);
    void calculateParameters(const Setting& setting);
    int _scan();
};

}
#endif // SCANNER_H
