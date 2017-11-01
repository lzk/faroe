#ifndef SCANNER_H
#define SCANNER_H
#include "scannerstruct.h"
#include "scannerapi.h"
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
class Scanner
{
public:
    typedef struct{
        int BitsPerPixel;
        int resolution;
        int width;
        int height;
        int x;
        int y;
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
        int nColPixelNumOrig;
        int nLinePixelNumOrig;
        int imgBufferSize;
        int bytesPerLine;
        unsigned char* imgBuffer;
    }
        Parameters;

public:
    Scanner();
    void install(DeviceIO* dio ,PlatformApp* platformApp);
    int ADFScan();
    void cancel();

    Setting* getSetting();
private:
    PlatformApp* pPlatformApp;
    bool m_cancel;
    ScannerAPI scannerApi;
    Parameters parameters;
    int waitJobFinish(int wait_motor_stop);
    int receiveData();
    Parameters getParameters();
    SC_PAR_DATA_T getScanParameters(const Setting& setting);
    void calculateParameters(const Setting& setting);
    int _scan();
};

}
#endif // SCANNER_H
