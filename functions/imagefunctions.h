#ifndef IMAGEFUNCTIONS_H
#define IMAGEFUNCTIONS_H

#include <QList>
#include <QObject>
class ImageFunctions :public QObject
{
//    Q_OBJECT
public:
    explicit ImageFunctions(QObject* parent = nullptr);
    virtual int preFunction(const QString& para = QString());
    virtual int function(const QStringList& ,const QString& para = QString());
    virtual int postFunction(const QString& para = QString());

public slots:
    void cancel(bool);

protected:
    bool m_cancel;
    QStringList m_fileList;
};

#include "../barcode/decodemanager.h"
class ImageFunctions_Separation : public ImageFunctions
{
public:
    virtual int preFunction(const QString&);
    virtual int function(const QStringList& ,const QString&);
    virtual int postFunction(const QString&);

private:
    struct Separation_data{
        QString barcodeString;
        QStringList fileList;
    };
    struct Separation_data* current_sd;
    QList<struct Separation_data> separation_data;
};

class ImageFunctions_Decode : public ImageFunctions
{
public:
    virtual int preFunction(const QString&);
    virtual int function(const QStringList& ,const QString&);
    virtual int postFunction(const QString&);

private:
    QList<struct DMDecodeResult> decode_data;
};

class ImageFunctions_ToPrint : public ImageFunctions
{
public:
    virtual int function(const QStringList& ,const QString&);
};

class ImageFunctions_ToEmail : public ImageFunctions
{
public:
    virtual int postFunction(const QString&);
};

class ImageFunctions_ToFile : public ImageFunctions
{
public:
    ImageFunctions_ToFile(bool quickscan);
    virtual int preFunction(const QString&);
    virtual int function(const QStringList& ,const QString&);
    virtual int postFunction(const QString&);

private:
    int currentPage;
    bool isQuickScan;
    QString fileName;
};

class ImageFunctions_ToApplication : public ImageFunctions
{
public:
    virtual int postFunction(const QString&);
};

class ImageFunctions_ToCloud : public ImageFunctions
{
public:
    virtual int preFunction(const QString&);
    virtual int function(const QStringList& ,const QString&);

};

#include <QUrl>
class QFtp;
class ImageFunctions_ToFtp :public ImageFunctions
{
public:
    ImageFunctions_ToFtp();
    ~ImageFunctions_ToFtp();
    virtual int preFunction(const QString&);
    virtual int function(const QStringList& ,const QString&);
    virtual int postFunction(const QString&);

//private slots:
//    void ftpDone(bool);

private:
    enum{
        CMD_connect,
        CMD_login,
        CMD_mkdir,
        CMD_cd,
        CMD_put,
        CMD_close,
        CMD_MAX
    };
    bool done;
    bool error;
    QFtp* ftp;
    QUrl ftpUrl;
    int waitForCmd(int ,const QString& para = QString() ,int time = 50);
};
#endif // IMAGEFUNCTIONS_H
