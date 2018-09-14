#ifndef IMAGEFUNCTIONS_H
#define IMAGEFUNCTIONS_H

#include <QList>
#include <QObject>
class ImageFunctions :public QObject
{
    Q_OBJECT
public:
    explicit ImageFunctions(QObject* parent = nullptr);
    virtual ~ImageFunctions(){}

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
    Q_OBJECT
public:
    explicit ImageFunctions_Separation(QObject* parent = nullptr)
        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_Separation(){}
    virtual int preFunction(const QString& para = QString());
    virtual int function(const QStringList& ,const QString& para = QString());
    virtual int postFunction(const QString& para = QString());

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
    Q_OBJECT
public:
    explicit ImageFunctions_Decode(QObject* parent = nullptr)
        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_Decode(){}
    virtual int preFunction(const QString& para = QString());
    virtual int function(const QStringList& ,const QString& para = QString());
    virtual int postFunction(const QString& para = QString());

private:
    QList<struct DMDecodeResult> decode_data;
};

class ImageFunctions_ToPrint : public ImageFunctions
{
    Q_OBJECT
public:
    explicit ImageFunctions_ToPrint(QObject* parent = nullptr)
        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_ToPrint(){}
//    virtual int preFunction(const QString& para = QString()){;}
    virtual int function(const QStringList& ,const QString& para = QString());
//    virtual int postFunction(const QString& para = QString()){;}
};

class ImageFunctions_ToEmail : public ImageFunctions
{
    Q_OBJECT
public:
    explicit ImageFunctions_ToEmail(QObject* parent = nullptr)
        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_ToEmail(){}
//    virtual int preFunction(const QString& para = QString()){;}
//    virtual int function(const QStringList& ,const QString& para = QString()){;}
    virtual int postFunction(const QString& para = QString());
};

class ImageFunctions_ToFile : public ImageFunctions
{
    Q_OBJECT
public:
//    explicit ImageFunctions_ToFile(QObject* parent = nullptr)
//        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_ToFile(){}
    virtual int preFunction(const QString& para = QString());
    virtual int function(const QStringList& ,const QString& para = QString());
    virtual int postFunction(const QString& para = QString());

    ImageFunctions_ToFile(bool quickscan ,QObject* parent = nullptr);
private:
    int currentPage;
    bool isQuickScan;
    QString fileName;
};

class ImageFunctions_ToApplication : public ImageFunctions
{
    Q_OBJECT
public:
//    explicit ImageFunctions_ToApplication(QObject* parent = nullptr)
//        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_ToApplication(){}
//    virtual int preFunction(const QString& para = QString()){;}
//    virtual int function(const QStringList& ,const QString& para = QString()){;}
    virtual int postFunction(const QString& para = QString());
};

class ImageFunctions_ToCloud : public ImageFunctions
{
    Q_OBJECT
public:
//    explicit ImageFunctions_ToCloud(QObject* parent = nullptr)
//        : ImageFunctions(parent){}
//    virtual ~ImageFunctions_ToCloud(){}
    virtual int preFunction(const QString& para = QString());
    virtual int function(const QStringList& ,const QString& para = QString());
//    virtual int postFunction(const QString& para = QString()){;}

};

#include <QUrl>
#include <Qtftp/QFtp>
#include <QTimer>
class ImageFunctions_ToFtp :public ImageFunctions
{
    Q_OBJECT
public:
    explicit ImageFunctions_ToFtp(QObject* parent = nullptr);
    virtual ~ImageFunctions_ToFtp();
    virtual int preFunction(const QString& para = QString());
    virtual int function(const QStringList& ,const QString& para = QString());
    virtual int postFunction(const QString& para = QString());

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
    bool timeout;
    QFtp* ftp;
    QUrl ftpUrl;
    QTimer timer;
    QFtp m_ftp;
    int waitForCmd(int ,const QString& para = QString() ,int time = 5);
};
#endif // IMAGEFUNCTIONS_H
