#ifndef JKENUMS_H
#define JKENUMS_H

#include <QObject>

class JKEnums : public QObject{
    Q_OBJECT

    Q_ENUMS(QuickScanMode)
    Q_ENUMS(EmailFileType)
    Q_ENUMS(SeparationFileType)
    Q_ENUMS(DecodeType)
    Q_ENUMS(ImageCommandResult)
    Q_ENUMS(PowerMode)
    Q_ENUMS(CommandPhase)
    Q_ENUMS(CommandResult)

public:
    enum QuickScanMode{
        ScanToPrint,
        ScanToFile,
        ScanToApplication,
        ScanToEmail,
        ScanToFTP,
        ScanToCloud,
    };
    enum EmailFileType{
        EmailFileType_PDF,
        EmailFileType_JPG,
    };
    enum SeparationFileType{
        SeparationFileType_PDF,
        SeparationfileType_TIFF,
    };
    enum DecodeType{
        DecodeType_Qrcode,
        DecodeType_OneD,
        DecodeType_All
    };

    enum ImageCommandState{
        ImageCommandState_start,
        ImageCommandState_processing,
        ImageCommandState_end
    };

    enum ImageCommandResult{
        ImageCommandResult_NoError = -100,
        ImageCommandResult_error_interface,
        ImageCommandResult_error_noProcessing,
        ImageCommandResult_error_processing,

        ImageCommandResult_error_saveFile,
        ImageCommandResult_error_invalidFilePath,

        ImageCommandResult_error_ftpConnect,
        ImageCommandResult_error_ftpLogin,
        ImageCommandResult_error_ftpCd,
        ImageCommandResult_error_ftpPut,
        ImageCommandResult_error_ftpTimeout,

        ImageCommandResult_error_icloudNotLogin,
        ImageCommandResult_error_icloudeUpload,

        ImageCommandResult_error_invalidPrinter,

        ImageCommandResult_error_cancel,
        ImageCommandResult_error_unknown,
    };
    enum PowerMode{
        PowerMode_unknown,
        PowerMode_ACPower,
        PowerMode_PowerBank,
        PowerMode_usbBusPower,
    };

    enum CommandPhase{
        CommandPhase_complelte,
        CommandPhase_start,
        CommandPhase_scanning,
        CommandPhase_processing,

    };

    enum ResultType{
        ResultType_common,
        ResultType_scanning,
        ResultType_processing,
        ResultType_decode,
        ResultType_separation,
        ResultType_ftp,
        ResultType_icloud,
    };
    enum CommandResult{
        CommandResult_NoError,
        CommandResult_error,
        CommandResult_error_cancel,
        CommandResult_error_unknown,

        CommandResult_error_invalidFilePath,
    };
};

#endif // JKENUMS_H
