#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class QFile;
class QNetworkAccessManager;
class ImageManager : public QObject
{
    Q_OBJECT
public:
    explicit ImageManager(QObject *parent = nullptr);

signals:
    void ftpUploadFile(QString);
    void ftpUploadComplete();
    void ftpError(QNetworkReply::NetworkError);

public slots:
    void imagesToPrint(QStringList ,QString);
    void imagesToEmail(QStringList ,int);
    void imagesToFile(QStringList ,QString);
    void imagesToFTP(QStringList ,QString);
    void imagesToApplication(QStringList ,QString);
    void imagesToCloud(QStringList ,QString);

private slots:
    void uploadDone();
    void errorhandler();
    void uploadProgress(qint64, qint64);
private:
    QString targetPathForFTP;
    QStringList fileListForFTP;
    QFile* fileForFTP;
    QNetworkReply *reply;
    QNetworkAccessManager* networkAccessManager;
    QUrl ftpUrl;

    void ftpUpload(QUrl& url);
};

#endif // IMAGEMANAGER_H
