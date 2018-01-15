#include "imagemanager.h"
#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>
#include <QUrl>
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
//#include "../newui/jkenums.h"

ImageManager::ImageManager(QObject *parent) : QObject(parent)
{

}

void ImageManager::imagesToPrint(QStringList fileList ,QString printerName)
{
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
        QString tmpFile = preFileName + "tmp.tif";
        QFile::remove(fullFileName);
        for (int i = 0 ;i < fileList.length() ;i++){
            QImage image(fileList[i]);
            image.save(tmpFile ,"tiff");
//            if(i){
//                QProcess().start("tiffutil" ,QStringList()<<"-cat"<<fullFileName<<tmpFile<<"-out"<<fullFileName);
//            }else{
//                QProcess().start("tiffutil" ,QStringList()<<"-cat"<<tmpFile<<"-out"<<fullFileName);
//            }
        }
    }
}

#include <QJsonDocument>
#include <QJsonObject>
void ImageManager::imagesToFTP(QStringList fileList,QString obj)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(obj.toLatin1()).object();

    QString serverAddress = jsonObj.value("serverAddress").toString();
    QString userName = jsonObj.value("userName").toString();
    QString password = jsonObj.value("password").toString();
    QString targetPath = jsonObj.value("targetPath").toString();

    QUrl url;
    url.setScheme("ftp");
    url.setHost(serverAddress);
    // 这里是设置远端主机上传时的文件名，不设置则名称为本地端上传名称
    url.setUserName(userName);
    url.setPassword(password);
    url.setPort(21);
    foreach (QString fileName, fileList) {
        url.setPath(targetPath + "/" + QFileInfo(fileName).fileName());
        qDebug()<<"FTP url:" <<url;
//        QFile file(fileName);
//        if(file.open(QIODevice::ReadOnly)){

//        }
    }
}

void ImageManager::imagesToApplication(QStringList ,QString)
{

}

void ImageManager::imagesToCloud(QStringList ,QString)
{

}
