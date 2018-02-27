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
        ImageCommandResult_NoError = 0,
        ImageCommandResult_error_interface = -100,
        ImageCommandResult_error_noProcessing,
        ImageCommandResult_error_processing,

        ImageCommandResult_error_saveFile,

        ImageCommandResult_error_ftpConnect,
        ImageCommandResult_error_ftpLogin,
        ImageCommandResult_error_ftpCd,
        ImageCommandResult_error_ftpPut,

        ImageCommandResult_error_icloudNotLogin,
        ImageCommandResult_error_icloudeUpload,

    };
};

#endif // JKENUMS_H
