#ifndef JKENUMS_H
#define JKENUMS_H

#include <QObject>

class JKEnums : public QObject{
    Q_OBJECT

    Q_ENUMS(QuickScanMode)

public:
    enum QuickScanMode{
        ScanToPrint,
        ScanToFile,
        ScanToApplication,
        ScanToEmail,
        ScanToFTP,
        ScanToCloud,
    };
};

#endif // JKENUMS_H
