#ifndef DEVICE_H
#define DEVICE_H

#define IMG_8_BIT	8
#define IMG_24_BIT	24
#define SCAN_FLB		IMG_FMT_FLB
#define SCAN_ADF		IMG_FMT_ADF
#define SCAN_A_SIDE		1
#define SCAN_B_SIDE		2
#define SCAN_AB_SIDE	3
enum{
    IMG_FMT_RAW = (('R'&0x000000ff) << 24) | (('A'&0x000000ff) << 16) | (('W'&0x000000ff)<< 8),
    IMG_FMT_JPG = (('J'&0x000000ff) << 24) | (('P'&0x000000ff) << 16) | (('G'&0x000000ff)<< 8),
    IMG_FMT_TIF = (('T'&0x000000ff) << 24) | (('I'&0x000000ff) << 16) | (('F'&0x000000ff)<< 8),
    IMG_FMT_BMP = (('B'&0x000000ff) << 24) | (('M'&0x000000ff) << 16) | (('P'&0x000000ff)<< 8),
    IMG_FMT_ADF = (('A'&0x000000ff) << 24) | (('D'&0x000000ff) << 16) | (('F'&0x000000ff)<< 8),
    IMG_FMT_FLB = (('F'&0x000000ff) << 24) | (('L'&0x000000ff) << 16) | (('B'&0x000000ff)<< 8),
};
#define SCAN_SOURCE   SCAN_ADF   //'FLB', 'ADF'
#define SCAN_ACQUIRE  ACQ_PICK_SS//ACQ_NO_SHADING
#define SCAN_OPTION   0
#define SCAN_DUPLEX   SCAN_AB_SIDE   // 1: A_side, 2: B_side, 3: AB_sides
#define SCAN_PAGE     0   // 0: adf sensor detect
#define IMG_FORMAT    IMG_FMT_JPG//I3('JPG')  //'JPG', 'RAW'
#define IMG_BIT       IMG_24_BIT      // 8, 16, 24, 48
#define IMG_MONO      IMG_COLOR

namespace JK {

enum{
    CMD_PRINT,
    CMD_SCAN,
    CMD_setWifi,
    CMD_setPassword,
    CMD_confirmPassword,
    CMD_setSaveTime,
    CMD_getSaveTime,
    CMD_getWifiInfo,
    CMD_getIpv4,
    CMD_setIpv4,
    CMD_setSoftap,
    CMD_getSoftap,
};

enum{
    ERR_ACK = 0,
    ERR_CMD_invalid = 1,
    ERR_Parameter_invalid = 2,
    ERR_Do_not_support = 3,
    ERR_Printer_busy = 4,
    ERR_Printer_error = 5,
    ERR_Set_parameter_error = 6,
    ERR_Get_parameter_error = 7,
    ERR_Printer_is_Sleeping = 8,
    ERR_Printer_is_in_error = 9,
    ERR_Password_incorrect = 0x0a,
    ERR_Scanner_operation_NG =0x0b,

    ERR_SCAN = 50,
    ERR_SCAN_ERRORDLL,
    ERR_SCAN_OPENFAIL,
    ERR_SCAN_ERRORPARAMETER,
    ERR_SCAN_NO_ENOUGH_SPACE,
    ERR_SCAN_ERROR_PORT,
    ERR_SCAN_CANCEL,
    ERR_SCAN_BUSY,
    ERR_SCAN_ERROR,
    ERR_SCAN_OPENFAIL_NET,
    ERR_SCAN_PAPER_JAM,
    ERR_SCAN_COVER_OPEN,
    ERR_SCAN_PAPER_NOT_READY,
    ERR_SCAN_CREATE_JOB_FAIL,
    ERR_SCAN_ADF_NOT_READY,
    ERR_SCAN_HOME_NOT_READY,

    //err self defined
    ERR_communication = -1,
    ERR_library = -2,
    ERR_decode_status = -3,
    ERR_wifi_have_not_been_inited = -4,
    ERR_decode_device = -5,
    ERR_printer_have_jobs = -6,
    ERR_WIFI_SET_SSID = -7,
    ERR_sane_scanning = -8,
    ERR_cmd_cannot_support = -10,
    ERR_invalid_data = -11,
    ERR_no_logined = -12,

    //status self defined
    STATUS_ready= 100,
    //warning
    STATUS_TonerEnd,
    STATUS_TonerNearEnd,
    STATUS_WasteTonerFull,
    //sleep
    STATUS_sleep,
    //busy
    STATUS_printing,
    STATUS_WarmingUp,
    STATUS_PrintCanceling,
    STATUS_Processing,
    STATUS_Copying,
    STATUS_CopyScanNextPage,
    STATUS_IDCardCopyTurnCardOver,
    STATUS_CopyCancelling,
    STATUS_IdCardMode,
    STATUS_Scanning,
    STATUS_ScanCanceling,
    STATUS_ScannerBusy,
    STATUS_ManualFeedRequired,
    STATUS_FWupdating,
    //warning
    STATUS_OverHeat,
    //Error
    STATUS_InitializeJam,
    STATUS_NofeedJam,
    STATUS_JamAtRegist_stayOn,
    STATUS_JamAtExit_notReach,
    STATUS_JamAtExit_stayOn,
    STATUS_CoverOpen,
    STATUS_NoTonerCartridge,
    STATUS_PDLmemoryOverflow,
//    STATUS_Error_TurnOffThePrinter,
    STATUS_PolygomotorOnTimeOutError_SC202,
    STATUS_PolygomotorOffTimeOutError_SC203,
    STATUS_PolygomotorLockTimeOutError_SC204,
    STATUS_BeamSynchronizeError_SC220,
    STATUS_BiasLeak_SC491,
    STATUS_PlateActionError_SC501,
    STATUS_MainmotorError_SC520,
    STATUS_MainFanMotorError_SC530,

//    STATUS_Error_contactCustomerSupport,
    STATUS_FuserThermistorError_SC541,
    STATUS_FuserReloadError_SC542,
    STATUS_HighTemperatureErrorSoft_SC543,
    STATUS_HighTemperatureErrorHard_SC544,
    STATUS_FuserFullHeaterError_SC545,
    STATUS_Fuser3timesJamError_SC559,

    STATUS_LowVoltageFuserReloadError_SC560,
    STATUS_MotorThermistorError_SC587,
    STATUS_EEPROMcommunicationError_SC669,
    STATUS_CTL_PRREQ_NsignalDoesntCome_SC688,

    STATUS_ScanMotorError_SC1001,
    STATUS_Scan_DRV_CALIB_FAIL_SC1001,
    STATUS_WirelessDongleConfigFail_WIFIconnect_SC1002,
    STATUS_WirelessDongleConfigFail_USBconnect_SC1002,
    STATUS_DMAError_SC1006,

//    STATUS_busy_scanningOrCoping,
//    STATUS_jam,
    STATUS_other,
};

class Scanner;
class DeviceIO;
class Setter;
class PlatformApp;
class Device
{
public:
    Device(const char* url ,DeviceIO* dio ,PlatformApp* platformApp);
    virtual ~Device();
    int scan();
    bool checkConnection();
    int deviceCmd(int cmd ,void* data);

    Scanner* getScanner();
private:
    DeviceIO* deviceIO;
    Scanner* scanner;
    Setter* setter;

    int open(const char* );
    int open();
    int close();
};

}
#endif // DEVICE_H
