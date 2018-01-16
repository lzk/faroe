#include "imagemanager.h"
#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>
#include <QUrl>
#include <QProcess>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
//#include "../newui/jkenums.h"

ImageManager::ImageManager(QObject *parent)
    : QObject(parent)
    ,networkAccessManager(new QNetworkAccessManager(this))
{
}

void ImageManager::imagesToPrint(QStringList fileList ,QString printerName)
{
    qDebug()<<"file list:"<<fileList;
    QPrinter _printer(QPrinterInfo::printerInfo(printerName));
    QPrinter* printer = &_printer;
    QPainter painter(printer);
    QRect rect = painter.viewport();
    for(int i = 0 ;i < fileList.length() ;i++) {
        QPixmap pixmap(fileList.at(i));
        QSize size = pixmap.size();
        if(!size.isValid())
            continue;
        if(i)
            printer->newPage();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0,0,pixmap);
    }
}

void ImageManager::imagesToEmail(QStringList ,int)
{

}

void ImageManager::imagesToFile(QStringList fileList ,QString filePath)
{
    QString fileName = QUrl(filePath).toLocalFile();
    QString fullFileName;
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();
    qDebug()<<"pre file name:" <<fileName;
    if(!suffix.compare("jpg")){
        for (int i = 0 ;i < fileList.length() ;i++){
            if(fileList.length() == 1)
                fullFileName = fileName;
            else
                fullFileName = preFileName + QString().sprintf("_%d." ,i) +suffix;
            qDebug()<<"save " << fileList[i];
            qDebug()<<"to " << fullFileName;
            QFile::copy(fileList[i] ,fullFileName);
        }
    }else if(!suffix.compare("bmp")){
        for (int i = 0 ;i < fileList.length() ;i++){
            if(fileList.length() == 1)
                fullFileName = fileName;
            else
                fullFileName = preFileName + QString().sprintf("_%d." ,i) +suffix;
            QImage image(fileList[i]);
            image.save(fullFileName ,"bmp");
        }
    }else if(!suffix.compare("pdf")){
        QPrinter print;
        QPrinter* printer = &print;
        qDebug()<<"printer:"<<printer->printerName();
//        printer->setPageSize(QPrinter::A4); //设置纸张大小为A4
        printer->setOutputFormat(QPrinter::PdfFormat); //设置输出格式为pdf
        printer->setOutputFileName(fileName);  //设置输出路径
        QPainter painter(printer);
        QRect rect = painter.viewport();
        for(int i = 0 ;i < fileList.length() ;i++) {
            QPixmap pixmap(fileList[i]);
            QSize size = pixmap.size();
            if(!size.isValid())
                continue;
            if(i)
                printer->newPage();
//            size.scale(rect.size(), Qt::KeepAspectRatio);
            printer->setPageSizeMM(size);
//            rect = painter.viewport();
//            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//            painter.setWindow(pixmap.rect());
            painter.drawPixmap(0,0,pixmap);
        }

    }else if(!suffix.compare("tif")){
#ifdef Q_OS_MAC
        QString tmpFile = preFileName + "tmp.tif";
        QFile::remove(fullFileName);
        for (int i = 0 ;i < fileList.length() ;i++){
            QImage image(fileList[i]);
            image.save(tmpFile ,"tiff");
            if(i){
                QProcess().execute("tiffutil" ,QStringList()<<"-cat"<<fullFileName<<tmpFile<<"-out"<<fullFileName);
            }else{
                QProcess().execute("tiffutil" ,QStringList()<<"-cat"<<tmpFile<<"-out"<<fullFileName);
            }
        }
        QFile::remove(tmpFile);
#endif
    }
}

void ImageManager::imagesToFTP(QStringList fileList,QString obj)
{
    if(!fileListForFTP.isEmpty())
        return;//have files not ftp upload
    QJsonObject jsonObj = QJsonDocument::fromJson(obj.toLatin1()).object();

    QString serverAddress = jsonObj.value("serverAddress").toString();
    QString userName = jsonObj.value("userName").toString();
    QString password = jsonObj.value("password").toString();
    QString targetPath = jsonObj.value("targetPath").toString();

    ftpUrl = QUrl(serverAddress);
    ftpUrl.setScheme("ftp");
//    ftpUrl.setHost(serverAddress);
    // 这里是设置远端主机上传时的文件名，不设置则名称为本地端上传名称
    ftpUrl.setUserName(userName);
    ftpUrl.setPassword(password);
    ftpUrl.setPort(21);

    targetPathForFTP = targetPath;
    fileListForFTP = fileList;
    ftpUpload(ftpUrl);
}

void ImageManager::ftpUpload(QUrl& url){
    QString fileName = fileListForFTP.first();
    url.setPath(targetPathForFTP + "/" + QFileInfo(fileName).fileName());
    fileForFTP = new QFile(fileName);
    if(fileForFTP->open(QIODevice::ReadOnly)){
        qDebug()<<"FTP url:" <<url;
        reply = networkAccessManager->put(QNetworkRequest(url) ,fileForFTP);
        connect(reply, SIGNAL(finished()), SLOT(uploadDone()));
//        connect(reply ,SIGNAL(error(QNetworkReply::NetworkError)) ,this ,SIGNAL(ftpError()));
        connect(reply ,SIGNAL(error(QNetworkReply::NetworkError)) ,SLOT(errorhandler()));
        connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));

        emit ftpUploadFile(url.path());
    }
}

void ImageManager::errorhandler()
{
    qDebug()<<"error:"<<reply->errorString();
    fileForFTP->deleteLater();
    reply->deleteLater();
}

void ImageManager::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug()<<"upload progress:"<<bytesSent <<"/"<<bytesTotal;
}

void ImageManager::uploadDone()
{
    qDebug() << "Finished" << reply->error();
    fileForFTP->deleteLater();
    reply->deleteLater();
    fileListForFTP.removeFirst();
    if(!fileListForFTP.isEmpty()){
        ftpUpload(ftpUrl);
    }else{
        emit ftpUploadComplete();
    }

}
void ImageManager::imagesToApplication(QStringList ,QString)
{

}

void ImageManager::imagesToCloud(QStringList ,QString)
{

}
