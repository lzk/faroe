#ifndef JKENUMS_H
#define JKENUMS_H

#include <QObject>

class JKEnums : public QObject{
    Q_OBJECT

    Q_ENUMS(QuickScanMode)
    Q_ENUMS(EmailFileType)

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
};

#endif // JKENUMS_H
