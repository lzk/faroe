#ifndef DEVICESTRUCT_H
#define DEVICESTRUCT_H

#include <QObject>

class DeviceStruct : public QObject {

    Q_OBJECT
    Q_ENUMS(CmdType)
    Q_ENUMS(StatusType)
    Q_ENUMS(ScanningProgress)

public:
    enum CmdType{
        CMD_PRINT,
        CMD_SCAN,
        CMD_setWifi,
        CMD_getWifiInfo,
        CMD_setPassword,
        CMD_confirmPassword,
        CMD_setSaveTime,
        CMD_getSaveTime,
        CMD_setOffTime,
        CMD_getOffTime,
        CMD_getIpv4,
        CMD_setIpv4,
        CMD_getSoftap,
        CMD_setSoftap,
        CMD_getDeviceSetting,
        CMD_setDeviceSetting,

        CMD_MAX,
        CMD_ScanTo,
        CMD_ScanTo_ToPrint,
        CMD_ScanTo_ToFile,
        CMD_ScanTo_ToApplication,
        CMD_ScanTo_ToEmail,
        CMD_ScanTo_ToFTP,
        CMD_ScanTo_ToCloud,
        CMD_QuickScan,
        CMD_QuickScan_ToPrint,
        CMD_QuickScan_ToFile,
        CMD_QuickScan_ToApplication,
        CMD_QuickScan_ToEmail,
        CMD_QuickScan_ToFTP,
        CMD_QuickScan_ToCloud,
        CMD_DecodeScan,
        CMD_SeperationScan,

        CMD_getPowerSupply,
        CMD_setPowerSaveTime,
        CMD_clearRollerCount,
        CMD_clearACMCount,
        CMD_doCalibration,
        CMD_Cloud_isLogin,
        CMD_Cloud_getFileList,
        CMD_Cloud_upload,
        CMD_Cloud_isExist,
    };

    enum StatusType{
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

        //err self defined
        ERR_SELF = -1000,
        ERR_communication,
        ERR_library,
        ERR_decode_status,
        ERR_wifi_have_not_been_inited,
        ERR_decode_device,
        ERR_printer_have_jobs,
        ERR_WIFI_SET_SSID,
        ERR_sane_scanning,
        ERR_cmd_cannot_support,
        ERR_invalid_data,
        ERR_no_logined,
        ERR_no_device,

        //err scan
        ERR_SCAN = 100,
        ERR_RETSCAN_ERRORDLL = ERR_SCAN + 1,
        ERR_RETSCAN_OPENFAIL = ERR_SCAN + 2,
        ERR_RETSCAN_ERRORPARAMETER = ERR_SCAN + 3,
        ERR_RETSCAN_NO_ENOUGH_SPACE = ERR_SCAN + 5,
        ERR_RETSCAN_ERROR_PORT = ERR_SCAN + 6,
        ERR_RETSCAN_CANCEL = ERR_SCAN + 7,
        ERR_RETSCAN_BUSY = ERR_SCAN + 8,
        ERR_RETSCAN_ERROR = ERR_SCAN + 9,
        ERR_RETSCAN_OPENFAIL_NET = ERR_SCAN + 10,
        ERR_RETSCAN_PAPER_JAM = ERR_SCAN + 11,
        ERR_RETSCAN_COVER_OPEN = ERR_SCAN + 12,
        ERR_RETSCAN_PAPER_NOT_READY = ERR_SCAN + 13,
        ERR_RETSCAN_CREATE_JOB_FAIL = ERR_SCAN + 14,
        ERR_RETSCAN_ADFCOVER_NOT_READY = ERR_SCAN + 15,
        ERR_RETSCAN_HOME_NOT_READY = ERR_SCAN + 16,
        ERR_RETSCAN_ULTRA_SONIC = ERR_SCAN + 17,
        ERR_RETSCAN_ERROR_POWER1 = ERR_SCAN + 18,
        ERR_RETSCAN_ERROR_POWER2 = ERR_SCAN + 19,
        ERR_RETSCAN_JOB_MISSING = ERR_SCAN + 20,
        ERR_RETSCAN_JOB_GOGING = ERR_SCAN + 21,
        ERR_RETSCAN_TIME_OUT = ERR_SCAN + 22,
        ERR_RETSCAN_USB_TRANSFERERROR = ERR_SCAN + 23,
        ERR_RETSCAN_WIFI_TRANSFERERROR = ERR_SCAN + 24,
        ERR_RETSCAN_ADFPATH_NOT_READY = ERR_SCAN + 25,
        ERR_RETSCAN_ADFDOC_NOT_READY = ERR_SCAN + 26,
        ERR_RETSCAN_GETINFO_FAIL = ERR_SCAN + 27,

        //status self defined
        STATUS = 1000,
        STATUS_ready,
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

    enum ScanningProgress{
        ScanningProgress_Start,
        ScanningProgress_Upload,
        ScanningProgress_Completed,
    };
};

#endif // DEVICESTRUCT_H
