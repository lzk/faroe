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

        ImageCommandResult_error_icloudNotLogin,
        ImageCommandResult_error_icloudeUpload,

        ImageCommandResult_error_invalidPrinter,

    };
    enum PowerMode{
        PowerMode_unknown,
        PowerMode_ACPower,
        PowerMode_PowerBank,
        PowerMode_usbBusPower,
    };
};

#endif // JKENUMS_H
