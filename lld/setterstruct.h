#ifndef SETTERSTRUCT_H
#define SETTERSTRUCT_H
namespace JK {
#pragma pack(1)

typedef signed char INT8;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct
{
    ////////////////////////////////////////////////////
    // Consumable
    ////////////////////////////////////////////////////
    BYTE	TonelStatusLevelK;
    BYTE	TonelStatusLevelC;
    BYTE	TonelStatusLevelM;
    BYTE	TonelStatusLevelY;
    BYTE	DrumStatusLifeRemain;

    ////////////////////////////////////////////////////
    // Covers
    ////////////////////////////////////////////////////
    BYTE	CoverStatusFlags;

    ////////////////////////////////////////////////////
    // Paper Tray
    ////////////////////////////////////////////////////
    BYTE	PaperTrayStatus;
    BYTE	PaperSize;

    ////////////////////////////////////////////////////
    // Output Tray
    ////////////////////////////////////////////////////
    BYTE	OutputTrayLevel;

    ////////////////////////////////////////////////////
    // General Status and information
    ////////////////////////////////////////////////////
    BYTE	PrinterStatus;
    WORD	OwnerName[16];
    WORD	DocuName[16];
    BYTE	ErrorCodeGroup;
    BYTE	ErrorCodeID;
    WORD	PrintingPage;
    WORD	Copies;
    DWORD	TotalCounter;

    BYTE         reserved[12];

    BYTE	TonerSize[4];
    BYTE	PaperType;
    BYTE	NonDellTonerMode;
    BYTE	AioStatus;
    BYTE	job;
    WORD	wReserved1;
    WORD	wReserved2;
} PRINTER_STATUS;

typedef struct _copycmdset
{
        UINT8 Density         ; // 0  -   0~6
        UINT8 copyNum         ; // 1  -   1~99
        UINT8 scanMode        ; // 2  -   0: Photo, 1: Text, 2: ID card
        UINT8 orgSize         ; // 3  -   0: A4, 1: A5, 2: B5, 3: Letter, 4: Executive
        UINT8 paperSize       ; // 4  -   0: Letter, 1: A4, 2: A5, 3: A6, 4: B5, 5: B6, 6: Executive, 7: 16K
        UINT8 nUp             ; // 5  -   0:1up, 1: 2up, 3: 4up, 4: 9up
        UINT8 dpi             ; // 6  -   0: 300*300, 1: 600*600
        UINT8 mediaType       ; // 7  -   0: plain paper 1: Recycled paper 2: Thick paper 3: Thin paper 4: Label
        UINT16 scale          ; // 8  -   25~400, disabled for 2/4/9up
} copycmdset;

typedef struct cmdst_userconfig
{
    INT8 LeadingEdge     ; // Leading Edge Registration
    INT8 SideToSide      ; // Side-to-size Registration
    INT8 ImageDensity    ; // Image Desity
    INT8 LowHumidityMode ; // Low Humidity Mode
    INT8 PlateControlMode;
    INT8 PrimaryCoolingMode;
} cmdst_userconfig;

typedef struct cmdst_softap
{
    UINT8 wifiEnable ; // bit0: Wi-Fi Enable, bit1: GO Enable, bit2: P2P Enable
    UINT8 reserved[7]; //
    char  ssid[32]   ; // used by both Legacy WiFi SSID and Wi-Fi Direct GO SSID
    char  pwd[64]    ; // used by both Legacy WiFi Passphrase & WEPKey and Wi-Fi Direct GO Passphrase
} cmdst_softap;

typedef struct cmdst_wifi_get
{
    UINT8 wifiEnable ; // bit0: Wi-Fi Enable, bit1: GO Enable, bit2: P2P Enable
    UINT8 sigLevel   ; //
    UINT8 reserved0  ; //
    UINT8 netType    ; // lenovo always 0
    UINT8 encryption ; // 0:No Security 1:WEP 64/128 bit 2.WPA-PSK-TKIP  3. WPA2-PSK-AES 4.Mixed Mode PSK
    UINT8 wepKeyId   ; //
    UINT8 reserved1  ; //
    UINT8 channel    ; //
    char  ssid[32]   ; // used by both Legacy WiFi SSID and Wi-Fi Direct GO SSID
    char  pwd[64]    ; // used by both Legacy WiFi Passphrase & WEPKey and Wi-Fi Direct GO Passphrase
    char  pinCode[8] ; //
    UINT8 reserved2[64]; //
    UINT8 ipAddr[4]  ; //
}cmdst_wifi_get;

typedef struct cmdst_wifi_set
{
    UINT8 wifiEnable ; // bit0: Wi-Fi Enable, bit1: GO Enable, bit2: P2P Enable
    UINT8 reserved0[2]; //
    UINT8 netType    ; //
    UINT8 encryption ; //
    UINT8 wepKeyId   ; //
    UINT8 reserved1; //
    UINT8 channel; //
    char  ssid[32]; // used by both Legacy WiFi SSID and Wi-Fi Direct GO SSID
    char  pwd[64]    ; // used by both Legacy WiFi Passphrase & WEPKey and Wi-Fi Direct GO Passphrase
}cmdst_wifi_set;


typedef struct _cmdst_user_center
{
    char	_2ndSerialNO[20];
    UINT32	_totalCounter;
    char	_SerialNO4AIO[16];
}cmdst_user_center;

typedef struct _fw_info_st
{
    char ServTag[32];
    char cPrinterSerialNumber[32];
    char cPrinterType[32];
    char cAssetTagNumber[32];
    char cMemoryCapacity[32];
    char cProcessorSpeed[32];
    char cFirmwareVersion[32];
    char cNetworkFirmwareVersion[32];
    char cEngFirmwareVersion[32];
    char cPrintingSpeedColor[32];
    char cPrintingSpeedMonochrome[32];
    char cBootCodeVersion[32];
    char cColorTableVersion[32];
    char cMacAddress[32];
} fw_info_st;

struct cmdst_aplist
{
    char ssid[33];
    UINT8 encryption;
};

#define NUM_OF_APLIST 10
typedef struct cmdst_aplist_get
{
    struct cmdst_aplist  aplist[NUM_OF_APLIST];
}
    cmdst_aplist_get;

typedef struct cmdst_passwd
{
    char passwd[32];
}
    cmdst_passwd;

typedef UINT8  cmdst_wifi_status;
typedef UINT8  cmdst_tonerEnd;
typedef UINT8  cmdst_PSave_time;
typedef UINT8  cmdst_powerOff_time;
typedef UINT8  cmdst_fusingScReset;

typedef struct net_info_st
{
    UINT8 IPMode            ; // 0-ipv4,1-ipv6
    UINT8 IPAddressMode     ; // 0 AutoIP,1 BOOTP,2 RARP,3 DHCP,4 Panel (Manual)
    UINT8 IPAddress[4]      ; // 0.0.0.0 ~ 223.255.255.255
    UINT8 SubnetMask[4]     ; // 0.0.0.0 ~ 223.255.255.255
    UINT8 GatewayAddress[4] ; // 0.0.0.0 ~ 223.255.255.255
} net_info_st;

typedef struct
{
    UINT8 UseManualAddress;
    char ManualAddress[40];
    UINT32 ManualMask;
    char StatelessAddress1[44];
    char StatelessAddress2[44];
    char StatelessAddress3[44];
    char LinkLocalAddress[40];
    char IPv6ManualGatewayAddress[40];
    char AutoGatewayAddress[40];
    char AutoStatefulAddress[44];
    UINT8 DHCPv6;
}net_ipv6_st;

//public enum EnumStatus: byte
enum
{
    Ready                       = 0x00,
    Printing                    = 0x01,
    PowerSaving                 = 0x02,
    WarmingUp                   = 0x03,
    PrintCanceling              = 0x04,
    Processing                  = 0x07,
    CopyScanning                = 0x60,
    CopyScanNextPage            = 0x61,
    CopyPrinting                = 0x62,
    CopyCanceling               = 0x63,
    IDCardMode                  = 0x64,
    ScanScanning                = 0x6A,
    ScanSending                 = 0x6B,
    ScanCanceling               = 0x6C,
    ScannerBusy                 = 0x6D,
    TonerEnd1                   = 0x7F,//For china maket
    TonerEnd2                   = 0x80,
    TonerNearEnd                = 0x81,
    ManualFeedRequired          = 0x85,
    InitializeJam               = 0xBC,
    NofeedJam                   = 0xBD,
    JamAtRegistStayOn           = 0xBE,
    JamAtExitNotReach           = 0xBF,
    JamAtExitStayOn             = 0xC0,
    CoverOpen                   = 0xC1,
    NoTonerCartridge            = 0xC5,
    WasteTonerFull              = 0xC6,
    PDLMemoryOver              = 0xC2,
    FWUpdate                    = 0xC7,
    OverHeat                    = 0xC8,
    PolygomotorOnTimeoutError   = 0xCD,
    PolygomotorOffTimeoutError  = 0xCE,
    PolygomotorLockSignalError  = 0xCF,
    BeamSynchronizeError        = 0xD1,
    BiasLeak                    = 0xD2,
    PlateActionError            = 0xD3,
    MainmotorError              = 0xD4,
    MainFanMotorEorror          = 0xD5,
    JoinerThermistorError       = 0xD6,
    JoinerReloadError           = 0xD7,
    HighTemperatureErrorSoft    = 0xD8,
    HighTemperatureErrorHard    = 0xD9,
    JoinerFullHeaterError       = 0xDA,
    Joiner3timesJamError        = 0xDB,
    LowVoltageJoinerReloadError = 0xDC,
    MotorThermistorError        = 0xDD,
    EEPROMCommunicationError    = 0xDE,
    CTL_PRREQ_NSignalNoCome     = 0xDF,
    SCAN_USB_Disconnect         = 0xE1,
    SCAN_NET_Disconnect         = 0xE4,
    ScanMotorError              = 0xE5,
    SCAN_DRV_CALIB_FAIL         = 0xE9,
    NetWirelessDongleCfgFail    = 0xE8,
    ScanDriverCalibrationFail   = 0xE9,
    DMAError                    = 0xEF,
    Unknown                     = 0xF0, // status added by SW
    Offline                     = 0xF1, // status added by SW
    PowerOff                    = 0xF2, // status added by SW
};

}
#endif // SETTERSTRUCT_H
