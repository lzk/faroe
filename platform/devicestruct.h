#ifndef DEVICESTRUCT_H
#define DEVICESTRUCT_H

#include <QObject>

class DeviceStruct : public QObject {

    Q_OBJECT
    Q_ENUMS(CmdType)
    Q_ENUMS(StatusType)

public:
    enum CmdType{
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
};

#endif // DEVICESTRUCT_H
