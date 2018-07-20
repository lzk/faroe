#include "imagefunctions.h"

#include <QDebug>
#include "../newui/jkenums.h"
#include "../platform/log.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QImage>
ImageFunctions::ImageFunctions(QObject* parent)
    : QObject(parent)
      ,m_cancel(false)
{
    m_fileList.clear();
}

int ImageFunctions::preFunction(const QString&)
{
    return 0;
}

int ImageFunctions::function(const QStringList& fileList,const QString&)
{
    m_fileList << fileList;
    return 0;
}

int ImageFunctions::postFunction(const QString&)
{
    return 0;
}

void ImageFunctions::cancel(bool ccl)
{
    m_cancel = ccl;
}

#include <QDir>
int ImageFunctions_Separation::preFunction(const QString& para)
{
    separation_data.clear();
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString filePath = jsonObj.value("filePath").toString();
    if(!filePath.isEmpty() && !QDir(filePath).exists()){
        return JKEnums::CommandResult_error_invalidFilePath;
    }
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_Separation::function(const QStringList& fileList ,const QString& para)
{
    ImageFunctions::function(fileList ,para);
    QString str;
    bool found;
    foreach (QString filename, fileList) {
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        str = DecodeManager().decodeOneD(filename);
        found = false;
        for(int i = 0 ;i < separation_data.count() ;i++){
            struct Separation_data sd = separation_data[i];
            if(sd.barcodeString == str){
                found = true;
                sd.fileList << filename;
                separation_data[i] = sd;
                break;
            }
        }
        if(!found){
            struct Separation_data sd;
            sd.barcodeString = str;
            sd.fileList << filename;
            separation_data << sd;
        }
    }
    return JKEnums::CommandResult_NoError;
}

#include "../platform/platform.h"
int ImageFunctions_Separation::postFunction(const QString& para)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    int fileType = jsonObj.value("fileType").toInt(0);
    QString filePath = jsonObj.value("filePath").toString();
    QString tmpFile = filePath + "/tmp.tif";
    QString fullFileName;
    int i=0 ,j=0;
    foreach (struct Separation_data sd, separation_data) {
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        fullFileName = filePath + QString().sprintf("/separation_%d.pdf" ,i);
        if(QFile::exists(fullFileName))
            QFile::remove(fullFileName);
        if(fileType == JKEnums::SeparationFileType_PDF){
            saveMultiPagePdfImageInit(fullFileName);
            j = 0;
            foreach (QString filename, sd.fileList) {
                saveMultiPagePdfImage(filename ,j == 0);
                j++;
            }
            saveMultiPagePdfImageRelease();
        }else{
            fullFileName = filePath + QString().sprintf("/separation_%d.tif" ,i);
            if(QFile::exists(fullFileName))
                QFile::remove(fullFileName);
            foreach (QString filename, sd.fileList) {
                if(QImage(filename).save(tmpFile ,"tiff")){
                    saveMultiPageTiffImage(tmpFile ,fullFileName);
                    QFile::remove(tmpFile);
                }
            }
        }
        i++;
        qDebug()<<"decode text:" << sd.barcodeString;
    }
    return JKEnums::CommandResult_NoError;
}


int ImageFunctions_Decode::preFunction(const QString&)
{
    decode_data.clear();
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_Decode::function(const QStringList& fileList ,const QString& para)
{
    ImageFunctions::function(fileList ,para);
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    int decodeType = jsonObj.value("codeType").toInt(0);
    struct DMDecodeResult dr;
    foreach (QString filename, fileList) {
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        switch (decodeType) {
        case JKEnums::DecodeType_Qrcode:
            dr = DecodeManager().decodeMultiQrcode(filename);
            break;
        case JKEnums::DecodeType_OneD:
            dr = DecodeManager().decodeMultiOneD(filename);
            break;
        case JKEnums::DecodeType_All:
            dr = DecodeManager().decodeMultiAll(filename);
            break;
        default:
            break;
        }
        if(!dr.fileName.isEmpty())
            decode_data << dr;
    }
    return JKEnums::CommandResult_NoError;
}

#include <QRegularExpression>
int ImageFunctions_Decode::postFunction(const QString& para)
{
    if(m_cancel){
        return JKEnums::CommandResult_error_cancel;
    }
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString fileName = jsonObj.value("fileName").toString();
    QString tmppath = getTempPath();
    fileName = tmppath + "/" + fileName;
    QFileInfo info(fileName);
    if(info.suffix() != "html")
        fileName += ".html";
    FILE* file = fopen(fileName.toLatin1().constData() ,"w");
    if(file == NULL)
        return JKEnums::CommandResult_error_unknown;
    const char* cFileName;
    const char* cText;
    bool firstLine;

    fprintf(file ,"<html> <head> <meta charset=\"UTF-8\">  <title>QRCode/Barcode Detection Result</title> </head>\n");
    fprintf(file ,"<style> table { font-family: \"Microsoft YaHei\"; font-size:12px } </style>\n");
    fprintf(file ,"<body>\n");
    fprintf(file ,"<H3>Result:</H3>\n");

    fprintf(file ,"<table border=\"1\" cellspacing=\"0\" vspace=\"0\" hspace=\"0\" cellpadding=\"10\">\n");
    fprintf(file ,"<tr>\n");
    fprintf(file ,"<th> File Name	     </th>\n");
    fprintf(file ,"<th> Source Bitmap	     </th>\n");
    fprintf(file ,"<th> Decode Bitmap	     </th>\n");
    fprintf(file ,"<th> Code Type	 </th>\n");
    fprintf(file ,"<th> Result Type	 </th>\n");
    fprintf(file ,"<th> Contents  	 </th>\n");
    fprintf(file ,"</tr>\n");
    foreach (struct DMDecodeResult dr, decode_data) {
        cFileName = QFileInfo(dr.fileName).fileName().toLatin1().constData();
        fprintf(file ,"<tr>\n");
        fprintf(file ,"<td rowspan=\"%d\">%s</td>\n"
                             ,dr.result.count() ,cFileName);
        if(dr.fileName.isEmpty()){
            fprintf(file ,"<td rowspan=\"%d\"></td>\n" ,dr.result.count());
        }else{
            int width,height;
            width = dr.width;
            height = dr.height;
            if(width > height){
                if(width > 300){
                    height = 300 * height / width;
                    width = 300;
                }
            }else{
                if(height > 300){
                    width = 300 * width / height;
                    height = 300;
                }
            }
            fprintf(file ,"<td rowspan=\"%d\"><img src=\"%s\" height=\"%d\" width=\"%d\"></td>\n"
                                 ,dr.result.count() ,cFileName ,height ,width);
        }

        if(dr.result.isEmpty()){
            fprintf(file ,"<td>Null</td>\n");
            fprintf(file ,"<td>N/A</td>\n");
            fprintf(file ,"<td>N/A</td>\n");
            fprintf(file ,"<td>Null</td>\n");
        }else{
            firstLine = true;
            foreach (DMResult result, dr.result) {
                if(!firstLine){
                    fprintf(file ,"<tr>\n");
                }
                qDebug()<<"decode text:"<<result.text;

                if(result.fileName.isEmpty()){
                    fprintf(file ,"<td>Null</td>\n");
                }else{
                    int width,height;
                    width = result.width;
                    height = result.height;
                    if(width > height){
                        if(width > 100){
                            height = 100 * height / width;
                            width = 100;
                        }
                    }else{
                        if(height > 100){
                            width = 100 * width / height;
                            height = 100;
                        }
                    }
                    fprintf(file ,"<td><img src=\"%s\" height=\"%d\" width=\"%d\"></td>\n"
                                         ,QFileInfo(result.fileName).fileName().toLatin1().constData() ,height ,width);
                }

                if(!result.text.isEmpty()){
                    fprintf(file ,"<td>%s</td>\n" ,result.format.toLatin1().constData());
                    bool isurl = false;
                    QString url = result.text.trimmed();
                    if(!url.contains(" ")){
                        QRegularExpression re_with_protocol("^[a-zA-Z][a-zA-Z0-9+-.]+:");
//                        QRegularExpression re_without_protocol("([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,}"
//                                                               +QString("(:\\d{1,5})?")
//                                                               +QString("(/|\\?|$)"));
                        if(re_with_protocol.match(url).hasMatch())
                            isurl = true;
//                        else if(re_without_protocol.match(url).hasMatch())
//                            isurl = true;
                    }
                    cText  = result.text.toUtf8().constData();//result.text.toLocal8Bit().constData();
//                    if(result.format != "RSS_14"){
                        if(isurl){
                            fprintf(file ,"<td>URI</td>\n");
                            fprintf(file ,"<td>  <a href=%s>%s</a> </td>\n" ,cText ,cText);
                        }else{
                            fprintf(file ,"<td>TEXT</td>\n");
                            fprintf(file ,"<td>%s</td>\n" ,cText);
                        }
//                    }else{
//                        fprintf(file ,"<td>N/A</td>\n");
//                        fprintf(file ,"<td>%s</td>\n" ,cText);
//                    }
                }else{
                    fprintf(file ,"<td>N/A</td>\n");
                    fprintf(file ,"<td>N/A</td>\n");
                    fprintf(file ,"<td>Null</td>\n");
                }

                if(!firstLine){
                    fprintf(file ,"</tr>\n");
                }else{
                    firstLine = false;
                }
            }
        }

        fprintf(file ,"</tr>\n");
    }
    fprintf(file ,"</body>\n");
    fprintf(file ,"</html>\n");
    fclose(file);
    system((QString("open ") + fileName).toLatin1().constData());
    return JKEnums::CommandResult_NoError;
}

#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>
int ImageFunctions_ToPrint::function(const QStringList& fileList ,const QString& para)
{
    ImageFunctions::function(fileList ,para);
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString printerName = jsonObj.value("printerName").toString();
    qDebug()<<"printer name:"<<printerName<<"\tfile list:"<<fileList;

    QStringList printerList = QPrinterInfo::availablePrinterNames();
    if(printerList.isEmpty() || !printerList.contains(printerName)){
        return JKEnums::ImageCommandResult_error_invalidPrinter;
    }
    QPrinter _printer(QPrinterInfo::printerInfo(printerName));
    QPrinter* printer = &_printer;
    QPainter painter;
    if(!printer->isValid() || !painter.begin(printer)){
        return JKEnums::ImageCommandResult_error_invalidPrinter;
    }
    QRect rect = painter.viewport();
    QPixmap pixmap;
    QSize size;
    for(int i = 0 ;i < fileList.length() ;i++) {
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        pixmap = QPixmap(fileList.at(i)).scaled(rect.size() ,Qt::KeepAspectRatio);
        size = pixmap.size();
        if(!size.isValid())
            continue;
        if(i)
            printer->newPage();
//        size.scale(rect.size(), Qt::KeepAspectRatio);
//        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//        painter.setWindow(pixmap.rect());
        painter.drawPixmap(rect.x() + (rect.width() - size.width()) / 2,
                           rect.y() + (rect.height() - size.height()) / 2,pixmap);
    }
    return JKEnums::CommandResult_NoError;
}

#include <QDateTime>
int ImageFunctions_ToEmail::postFunction(const QString& para)
{
    if(m_fileList.isEmpty())
        return -1;
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    int fileType = jsonObj.value("fileType").toInt(0);
    QString subject =  jsonObj.value("subject").toString();
    QString recipient =  jsonObj.value("recipient").toString();
    switch (fileType) {
    case JKEnums::EmailFileType_PDF:{
        QString filename = getTempPath() + "/vopimg" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".pdf";
        saveMultiPagePdfImageInit(filename);
        bool first = true;
        foreach (QString tmpFileName, m_fileList) {
            if(m_cancel)
                break;
            saveMultiPagePdfImage(tmpFileName ,first);
            first = false;
        }
        saveMultiPagePdfImageRelease();
        if(!m_cancel)
            openEmail(subject ,recipient ,QStringList()<<filename);
    }
        break;
    case JKEnums::EmailFileType_JPG:
    default:
        openEmail(subject ,recipient ,m_fileList);
        break;
    }
    return JKEnums::CommandResult_NoError;
}

ImageFunctions_ToFile::ImageFunctions_ToFile(bool quickscan)
    :isQuickScan(quickscan)
{
}

int ImageFunctions_ToFile::preFunction(const QString& para)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
//    if(isQuickScan){
//        QString filePath = jsonObj.value("filePath").toString();
//        if(!QDir(filePath).exists()){
//            return JKEnums::ImageCommandResult_error_invalidFilePath;
//        }
//        QString fileName_ = jsonObj.value("fileName").toString();
//        QString fullPath = filePath + "/" + fileName_  + "_" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
//        int fileType = jsonObj.value("fileType").toInt(0);
//        switch (fileType) {
//        case 0:     fullPath += ".pdf";    break;
//        case 1:     fullPath += ".tif";    break;
//        case 2:     fullPath += ".jpg";    break;
//        case 3:     fullPath += ".bmp";    break;
//        default:
//            break;
//        }
//        fileName = fullPath;
//    }else{
//        fileName = para;
//    }
    fileName = jsonObj.value("fullFileName").toString();

    qDebug()<<"save file name:"<<fileName;
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    int maxLength = 255;
    if(suffix == "pdf"){
    }else if(suffix == "tif"){
    }else{
        maxLength = 250;
    }
    if(fileInfo.fileName().length() > maxLength){
        return JKEnums::ImageCommandResult_error_invalidFilePath;
    }

    if(suffix == "pdf"){
        if(QFile::exists(fileName)){
            QFile::remove(fileName);
        }
        saveMultiPagePdfImageInit(fileName);
    }else if(suffix == "tif"){
        if(QFile::exists(fileName)){
            QFile::remove(fileName);
        }
    }

    currentPage = 0;
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_ToFile::postFunction(const QString&)
{
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    if(suffix == "pdf"){
        saveMultiPagePdfImageRelease();
    }else if(suffix == "bmp" || suffix == "jpg"){
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        if(currentPage == 1){
            QString fileName_ = isQuickScan ?"_" :"";
            QString fullFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName() + fileName_ + "1." + suffix;
            QFile::rename(fullFileName ,fileName);
        }
    }
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_ToFile::function(const QStringList& fileList ,const QString& para)
{
    ImageFunctions::function(fileList ,para);
    bool success = true;
    QString fullFileName;
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();
    qDebug()<<"pre file name:" <<preFileName;

    QString tmpFile = getTempPath() + "/tmp.tif";
    QString fileName_ = isQuickScan ?"_" :"";
    for (int i = 0 ;i < fileList.length() ;i++){
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        if(suffix == "jpg"){
            fullFileName = preFileName + fileName_ + QString().sprintf("%d." ,currentPage + 1) +suffix;
            if(QFile::exists(fullFileName))
                QFile::remove(fullFileName);
            if(!QFile::copy(fileList[i] ,fullFileName)){
                success = false;
                break;
            }
        }else if(suffix == "bmp"){
            fullFileName = preFileName + fileName_ + QString().sprintf("%d." ,currentPage + 1) +suffix;
            if(!QImage(fileList[i]).save(fullFileName ,"bmp")){
                success = false;
                break;
            }
        }else if(suffix == "pdf"){
            if(!saveMultiPagePdfImage(fileList[i] ,currentPage == 0)){
                success = false;
                break;
            }
        }else if(suffix == "tif"){
            if(QImage(fileList[i]).save(tmpFile ,"tiff")){
                saveMultiPageTiffImage(tmpFile ,fileName);
                QFile::remove(tmpFile);
            }else{
                success = false;
                break;
            }
        }
        currentPage++;
    }
    if(!success){
        return JKEnums::ImageCommandResult_error_saveFile;
    }
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_ToApplication::postFunction(const QString& para)
{
    if(m_fileList.isEmpty())
        return -1;
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
//    int fileType = jsonObj.value("fileType").toInt(0);
//    QString fileName;
//    QString filePath;
    QString fullFileName;
    fullFileName = jsonObj.value("fullFileName").toString();
//    if(fileType){
////        fileName =  jsonObj.value("fileName").toString();
////        filePath =  jsonObj.value("filePath").toString();
//    }else{
////        filePath = "/Applications";
////        fileName = "Preview.app";
//        fullFileName = jsonObj.value("fullFileName").toString();
//    }
    openApplication(fullFileName ,m_fileList);
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_ToCloud::preFunction(const QString& para)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
    if(!cloudTypeText.compare("iCloud")){
        QString str;
        if(!iCloudCheckLogin(str)){
            return JKEnums::ImageCommandResult_error_icloudNotLogin;
        }
    }
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_ToCloud::function(const QStringList& fileList,const QString& para)
{
    ImageFunctions::function(fileList ,para);
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
    if(!cloudTypeText.compare("iCloud")){
        foreach (QString fileName, fileList) {
            if(m_cancel){
                return JKEnums::CommandResult_error_cancel;
            }
            if(!iCloudUpload(fileName)){
                return JKEnums::ImageCommandResult_error_icloudeUpload;
            }
        }
    }
    return JKEnums::CommandResult_NoError;
}

#include <QThread>
#include <QCoreApplication>
#include <QUrl>
#include <Qtftp/QFtp>
ImageFunctions_ToFtp::ImageFunctions_ToFtp()
    :
      error(false)
    ,ftp(NULL)
{
}
ImageFunctions_ToFtp::~ImageFunctions_ToFtp()
{
    if(ftp)
        delete ftp;
}


int ImageFunctions_ToFtp::preFunction(const QString& para)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString serverAddress = jsonObj.value("serverAddress").toString();
    QString userName = jsonObj.value("userName").toString();
    QString password = jsonObj.value("password").toString();
    QString targetPath = jsonObj.value("targetPath").toString().remove(0 ,1);

    QString host;
    if(serverAddress.startsWith("ftp://")){
        host = serverAddress.mid(6);
    }else{
        host = serverAddress;
    }

    ftpUrl = QUrl(serverAddress);
    ftpUrl.setScheme("ftp");
    ftpUrl.setHost(host);
    ftpUrl.setUserName(userName);
    ftpUrl.setPassword(password);
    ftpUrl.setPath(targetPath);

    if(ftp){
        ftp->deleteLater();
    }
    ftp = new QFtp();
//    connect(ftp ,&QFtp::done ,this ,&ImageFunctions_ToFtp::ftpDone);
    QObject::connect(ftp ,&QFtp::done ,[=](bool error){this->error = error;done=true;});

    int error;
    error = waitForCmd(CMD_connect);
    if(error)
        return error;
    error = waitForCmd(CMD_login);
    if(error)
        return error;
    error = waitForCmd(CMD_mkdir);
    error = waitForCmd(CMD_cd);
    return error;
}

int ImageFunctions_ToFtp::function(const QStringList& fileList,const QString& para)
{
    ImageFunctions::function(fileList ,para);
    int error;
    foreach (QString fileName, fileList) {
        error = waitForCmd(CMD_put ,fileName ,150);
        if(error)
            break;
    }
    return error;
}

int ImageFunctions_ToFtp::postFunction(const QString&)
{
    if(ftp){
        delete ftp;
        ftp = NULL;
    }
    return JKEnums::CommandResult_NoError;
}

int ImageFunctions_ToFtp::waitForCmd(int cmd ,const QString& para ,int time)
{
    QFile* file = NULL;
    switch (cmd) {
    case CMD_connect:
        LOG_NOPARA("connect host:" + ftpUrl.host());
        ftp->connectToHost(ftpUrl.host());
//        ftp->connectToHost(ftpUrl.host() ,ftpUrl.port());
        break;
    case CMD_login:
        ftp->login(QUrl::fromPercentEncoding(ftpUrl.userName().toLatin1()), ftpUrl.password());
        break;
    case CMD_cd:
        ftp->cd(ftpUrl.path());
        break;
    case CMD_mkdir:
        ftp->mkdir(ftpUrl.path());
        break;
    case CMD_put:{
        QString fileName = para;
        QString remoteFileName = QFileInfo(fileName).fileName();
        file = new QFile(fileName);
        file->setParent(ftp);
        ftp->put(file ,remoteFileName);
    }
        break;
    default:
        return JKEnums::CommandResult_error_unknown;
    }
    done = false;
    LOG_PARA("ftp command:%d" ,cmd);
    for(int i = 0 ;i < time ;i++){
        if(m_cancel){
            return JKEnums::CommandResult_error_cancel;
        }
        QCoreApplication::processEvents();
        if(done){
            if(error){
                switch (cmd) {
                case CMD_connect:
                    return JKEnums::ImageCommandResult_error_ftpConnect;
                case CMD_login:
                    return JKEnums::ImageCommandResult_error_ftpLogin;
                case CMD_cd:
                    return JKEnums::ImageCommandResult_error_ftpCd;
                case CMD_put:
                    if(file)
                        file->deleteLater();
                    return JKEnums::ImageCommandResult_error_ftpPut;
                case CMD_mkdir:
                default:
                    return JKEnums::CommandResult_NoError;
                }
            }else{
                return JKEnums::CommandResult_NoError;
            }
        }
        QThread::msleep(100);
    }
    return JKEnums::ImageCommandResult_error_ftpTimeout;
}

//void ImageFunctions_ToFtp::ftpDone(bool error){
//    this->error = error;
//    done = true;
//}
