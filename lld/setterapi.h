#ifndef SETTERAPI_H
#define SETTERAPI_H
namespace JK {

typedef enum _CMD_ID
{
    _LS_SEARCH		= 0x0000, 	// Search Machine
    _LS_CONNECT		= 0x0001,	// Connect Machine
    _LS_DISCONNECT	= 0x0002,	// Disconnect Machine
    _LS_CALLFUNC	= 0x0100,	// Call Function
    _LS_SHOWVAR		= 0x0101, 	// Show Variable/Structure
    _LS_MODIFYVAR	= 0x0102,	// Modify Variable/Structure
    _LS_PRIVEXEC	= 0x0103,	// Private Execution
    _LS_ENGCMD		= 0x0104,	// Engine Command
    _LS_NETCMD		= 0x0105,	// Network Command
    _LS_WIFICMD		= 0x0106,	// Wireless Command
    _LS_PRNCMD		= 0x0107,	// Print Command
    _LS_SCACMD		= 0x0108,	// Scan Command
    _LS_CPYCMD		= 0x0109,	// Copy Command
    _LS_FAXCMD		= 0x010A,	// Fax Command
    _LS_DBGMSG		= 0x0200, 	// Debug Message
    _LS_HEARTBEAT 	= 0x0201,	// Heart Beat, Null Packet, keep activated
    _LS_PANKEY		= 0x0300,	// Panel Key Simulation
    _LS_PANIMG		= 0x0301,	// Panel Frame & LED status
    _LS_DATADOWN	= 0x0400,	// Download Data
    _LS_DATAUPLD	= 0x0401	// Upload Data
}CMD_ID;
#define CODE(x) ((x) << 16)
#define GET_CMDCODE(x) ((x) >> 16)
#define GET_SUBCMDCODE(x) ((x) & 0xffff)
typedef enum _CMD_CODE{
    CMD_CODE_copy_get = CODE(_LS_CPYCMD),
    CMD_CODE_copy,

    CMD_CODE_getWifiInfo = CODE(_LS_WIFICMD),
    CMD_CODE_setWifiInfo,
    CMD_CODE_getApList = CODE(_LS_WIFICMD) + 7,
    CMD_CODE_getWifiStatus,
    CMD_CODE_get_softAp = CODE(_LS_WIFICMD) + 0x10,
    CMD_CODE_set_softAp,
    CMD_CODE_setWifiInfo_noRead = CODE(_LS_WIFICMD) + 0xff,//self defined

    CMD_CODE_getPsaveTime = CODE(_LS_PRNCMD),
    CMD_CODE_setPsaveTime,
    CMD_CODE_get_userConfig,
    CMD_CODE_set_userConfig,
    CMD_CODE_setPasswd = CODE(_LS_PRNCMD) + 6,
    CMD_CODE_getPasswd,
    CMD_CODE_confirmPasswd,
    CMD_CODE_fusingScReset = CODE(_LS_PRNCMD) + 0x0b,
    CMD_CODE_getPowerOff = CODE(_LS_PRNCMD) + 0x0e,
    CMD_CODE_setPowerOff,
    CMD_CODE_getTonerEnd = CODE(_LS_PRNCMD) + 0x11,
    CMD_CODE_setTonerEnd,

    CMD_CODE_getv4 = CODE(_LS_NETCMD),
    CMD_CODE_setv4,
    CMD_CODE_getv6,
    CMD_CODE_setv6,

}
    CMD_CODE;

class DeviceIO;
class SetterApi
{
public:
    SetterApi();
    void install(DeviceIO* dio);
    int cmd(int cmd ,void* data ,int data_size);

private:
    DeviceIO* dio;
    int writeThenRead(char* wrBuffer ,int wrSize ,char* rdBuffer ,int rdSize);
    int write(char* wrBuffer ,int wrSize);
    int _cmd(int cmd ,void* data ,int data_size);
};

}
#endif // SETTERAPI_H
