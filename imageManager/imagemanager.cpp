#include "imagemanager.h"
#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>
#include <QUrl>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <Qtftp/QFtp>
#include <QDir>
#include "../platform/devicestruct.h"
#include "../newui/jkenums.h"
#include "../platform/platform.h"
#include "../imageManager/decodemanager.h"

ImageManager::ImageManager(QObject *parent)
    : QObject(parent)
    ,cmd_status(0)
    ,decodeManager(new DecodeManager(this))
{
}

void ImageManager::init()
{
    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
}

void ImageManager::imagesCmdStart(int cmd, QString para ,QStringList)
{
    cmd_state = JKEnums::ImageCommandState_start;
    if(cmd_status){//processing
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_noProcessing);
        return;
    }
    qDebug()<<"image cmd:"<<cmd;
    qDebug()<<"image cmd para"<<para;
    this->cmd = cmd;
    this->fileList.clear();
    cmd_status = 1;
    result = JKEnums::ImageCommandResult_NoError;
    cmd_para =  para;
    currentPage = 0;
    switch (cmd) {
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_ScanTo_ToFTP:
        ftpStart(para);
        break;
    case DeviceStruct::CMD_QuickScan_ToFile:
        fileName_ = "";
        saveToFileStart(para);
        break;
    case DeviceStruct::CMD_ScanTo_ToFile:
        fileName_ = "_";
        saveToFileStart(para);
        break;
    case DeviceStruct::CMD_SeperationScan:
        separation_data.clear();
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    case DeviceStruct::CMD_DecodeScan:
        separationScanDecodeStart();
        break;
    case DeviceStruct::CMD_ScanTo_ToCloud:
    case DeviceStruct::CMD_QuickScan_ToCloud:
        cloudStart(para);
        break;

    case DeviceStruct::CMD_QuickScan_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToEmail:
    case DeviceStruct::CMD_ScanTo_ToApplication:
    case DeviceStruct::CMD_QuickScan_ToApplication:
        default:
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    }
}

void ImageManager::cmdResult(int cmd ,int result)
{
    cmd_state = JKEnums::ImageCommandState_end;
    if(cmd_status == 0){
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_noProcessing);
        return;
    }
    cmd_status = 0;
    emit imagesCommandResult(cmd ,cmd_state ,result);
}

void ImageManager::imagesCmdEnd(int cmd ,int result)
{
    this->result = result;
    switch (cmd) {
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_ScanTo_ToFTP:
        ftpEnd();
        break;
    case DeviceStruct::CMD_QuickScan_ToFile:
    case DeviceStruct::CMD_ScanTo_ToFile:
//        if(!result || result == JKEnums::ImageCommandResult_NoError){
            saveToFileEnd(cmd_para);
//        }
        cmdResult(cmd ,result);
        break;
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToEmail:
        if(!result || result == JKEnums::ImageCommandResult_NoError){
            toEmail(cmd_para);
        }
        cmdResult(cmd ,result);
        break;
    case DeviceStruct::CMD_ScanTo_ToApplication:
    case DeviceStruct::CMD_QuickScan_ToApplication:
        if(!result || result == JKEnums::ImageCommandResult_NoError){
            toApplication(cmd_para ,fileList);
        }
        cmdResult(cmd ,result);
        break;
    case DeviceStruct::CMD_SeperationScan:
        if(!result || result == JKEnums::ImageCommandResult_NoError){
            separationScanDecodeEnd();
        }
        cmdResult(cmd ,result);
        break;
    case DeviceStruct::CMD_DecodeScan:
        if(!result || result == JKEnums::ImageCommandResult_NoError){
            decodeScanEnd();
        }
        cmdResult(cmd ,result);
        break;
    case DeviceStruct::CMD_ScanTo_ToCloud:
    case DeviceStruct::CMD_QuickScan_ToCloud:

    case DeviceStruct::CMD_QuickScan_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToPrint:
    default:
        cmdResult(cmd ,result);
        break;
    }
}

void ImageManager::imagesCmd(QStringList fileList)
{
    if(fileList.isEmpty())
        return;
    cmd_state = JKEnums::ImageCommandState_processing;
    if(cmd_status == 0){
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_noProcessing);
        return;
    }

    this->fileList << fileList;
    switch (cmd) {
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToEmail:
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    case DeviceStruct::CMD_QuickScan_ToApplication:
    case DeviceStruct::CMD_ScanTo_ToApplication:
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    case DeviceStruct::CMD_QuickScan_ToFile:
    case DeviceStruct::CMD_ScanTo_ToFile:
        if(saveToFile(fileList ,cmd_para)){
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        }else{
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_saveFile);
        }
        break;
    case DeviceStruct::CMD_ScanTo_ToPrint:
    case DeviceStruct::CMD_QuickScan_ToPrint:
        if(toPrint(fileList ,cmd_para)){
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        }else{
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_invalidPrinter);
        }
        break;

    case DeviceStruct::CMD_ScanTo_ToFTP:
        foreach (QString filename, fileList) {
            ftpUpload(filename);
        }
        ftpEnd();
        break;
    case DeviceStruct::CMD_QuickScan_ToFTP:
        foreach (QString filename, fileList) {
            ftpUpload(filename);
        }
        break;
    case DeviceStruct::CMD_ScanTo_ToCloud:
    case DeviceStruct::CMD_QuickScan_ToCloud:
        toCloud(fileList ,cmd_para);
        break;
    case DeviceStruct::CMD_SeperationScan:
        separationScanDecode(fileList);
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    case DeviceStruct::CMD_DecodeScan:
        decodeScanDecode(fileList);
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    default:
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        break;
    }
}

bool ImageManager::toPrint(const QStringList& fileList ,const QString& data)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(data.toLatin1()).object();
    QString printerName = jsonObj.value("printerName").toString();
    qDebug()<<"printer name:"<<printerName<<"\tfile list:"<<fileList;

    QStringList printerList = QPrinterInfo::availablePrinterNames();
    if(printerList.isEmpty() || !printerList.contains(printerName)){
        return false;
    }
    QPrinter _printer(QPrinterInfo::printerInfo(printerName));
    QPrinter* printer = &_printer;
    QPainter painter;
    cmd_state = JKEnums::ImageCommandState_processing;
    if(!printer->isValid() || !painter.begin(printer)){
        return false;
    }
    QRect rect = painter.viewport();
    QPixmap pixmap;
    QSize size;
    for(int i = 0 ;i < fileList.length() ;i++) {
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
    return true;
}

void ImageManager::toEmail(const QString& para)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    int fileType = jsonObj.value("fileType").toInt(0);
    QString subject =  jsonObj.value("subject").toString();
    QString recipient =  jsonObj.value("recipient").toString();
    switch (fileType) {
    case JKEnums::EmailFileType_PDF:{
        QString filename = getTempPath() + "/vopimg" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".pdf";
        saveMultiPagePdfImageInit(filename);
        bool first = true;
        foreach (QString tmpFileName, fileList) {
            saveMultiPagePdfImage(tmpFileName ,first);
            first = false;
        }
        saveMultiPagePdfImageRelease();
        openEmail(subject ,recipient ,QStringList()<<filename);
    }
        break;
    case JKEnums::EmailFileType_JPG:
    default:
        openEmail(subject ,recipient ,fileList);
        break;
    }
}

void ImageManager::saveToFileStart(const QString& fileName)
{
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
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_invalidFilePath);
        return;
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
    emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
}

void ImageManager::saveToFileEnd(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    if(suffix == "pdf"){
        saveMultiPagePdfImageRelease();
    }else if(suffix == "bmp" || suffix == "jpg"){
        if(currentPage == 1){
            QString fullFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName() + fileName_ + "1." + suffix;
            QFile::rename(fullFileName ,fileName);
        }
    }
}

bool ImageManager::saveToFile(const QStringList& fileList ,const QString& fileName)
{
    bool success = true;
    QString fullFileName;
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();
    qDebug()<<"pre file name:" <<preFileName;

    QString tmpFile = getTempPath() + "/tmp.tif";
    for (int i = 0 ;i < fileList.length() ;i++){
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
    return success;
}

#include <QTcpSocket>
void ImageManager::ftpStart(const QString& para)
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
    QTcpSocket socket;
    socket.connectToHost(host,21);
    if(!socket.waitForConnected(5000))
    {
        qDebug()<<"ftp tcp error:"<< socket.error();
        qDebug()<< socket.errorString();
        socket.close();
        result = JKEnums::ImageCommandResult_error_ftpConnect;
        cmdResult(cmd ,result);
        return;
    }
    socket.close();

    ftpUrl = QUrl(serverAddress);
    ftpUrl.setScheme("ftp");
    ftpUrl.setHost(host);
    ftpUrl.setUserName(userName);
    ftpUrl.setPassword(password);
    ftpUrl.setPath(targetPath);

//    ftp->connectToHost(ftpUrl.host());
    ftp->connectToHost(host);
}

void ImageManager::ftpEnd()
{
    if(ftp->state() != QFtp::Closing)
        ftp->close();
}

void ImageManager::ftpUpload(const QString& fileName)
{
    if(fileName.isEmpty())
        return;
    QString remoteFileName = QFileInfo(fileName).fileName();
    struct Private_data ftpData;
    ftpData.file = new QFile(fileName);
    ftpData.file->setParent(ftp);
//    if(ftpData.file->open(QIODevice::ReadOnly)){
        ftpData.id = ftp->put(ftpData.file ,remoteFileName);
        ftp_data << ftpData;
//    }
}

void ImageManager::ftpCommandFinished(int id ,bool error)
{
    switch (ftp->currentCommand()) {
    case QFtp::ConnectToHost:
        if(!error){
            qDebug()<<"connect to host finished";
            qDebug()<<"user name:"<<ftpUrl.userName() <<" password:" << ftpUrl.password();
            if (!ftpUrl.userName().isEmpty())
                ftp->login(QUrl::fromPercentEncoding(ftpUrl.userName().toLatin1()), ftpUrl.password());
            else{
                result = JKEnums::ImageCommandResult_error_ftpLogin;
                emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_ftpLogin);
            }
        }else{
            result = JKEnums::ImageCommandResult_error_ftpConnect;
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_ftpConnect);
        }
        break;
    case QFtp::Login:
        if(!error){
            qDebug()<<"login finished";
            ftp->mkdir(ftpUrl.path());
        }else{
            result = JKEnums::ImageCommandResult_error_ftpLogin;
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_ftpLogin);
        }
        break;
    case QFtp::Mkdir:
        qDebug()<<"mkdir finished error:"<<error;
        ftp->cd(ftpUrl.path());
        break;
    case QFtp::Cd:
        if(!error){
            qDebug()<<"cd finished";
            cmd_state = JKEnums::ImageCommandState_start;
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        }else{
            result = JKEnums::ImageCommandResult_error_ftpCd;
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_ftpCd);
        }
        break;
    case QFtp::Put:{
        foreach (struct Private_data data, ftp_data) {
            if(data.id == id){
                data.file->deleteLater();
                break;
            }
        }
        qDebug()<<"put finished error:"<<error;
        cmd_state = JKEnums::ImageCommandState_processing;
        if(!error){
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        }else{
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_ftpPut);
            result = JKEnums::ImageCommandResult_error_ftpPut;
        }
    }
        break;
    case QFtp::Close:
        qDebug()<<"ftp close error:"<<error;
        cmdResult(cmd ,result);
        break;
    default:
        break;
    }
}

void ImageManager::toApplication(const QString& para,const QStringList& fileList)
{
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
    openApplication(fullFileName ,fileList);
}

void ImageManager::cloudStart(const QString& para)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
    if(!cloudTypeText.compare("iCloud")){
        cmd_state = JKEnums::ImageCommandState_start;
        QString str;
        if(iCloudCheckLogin(str)){
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
        }else{
            emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_icloudNotLogin);
        }
    }
}

void ImageManager::toCloud(const QStringList& fileList,const QString& para)
{
    cmd_state = JKEnums::ImageCommandState_processing;
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
    if(!cloudTypeText.compare("iCloud")){
        foreach (QString fileName, fileList) {
            if(!iCloudUpload(fileName)){
                emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_icloudeUpload);
                return;
            }
        }
    }
    emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
}

void ImageManager::separationScanDecodeStart()
{
    decode_data.clear();
    QJsonObject jsonObj = QJsonDocument::fromJson(cmd_para.toLatin1()).object();
    QString filePath = jsonObj.value("filePath").toString();
    if(!QDir(filePath).exists()){
        emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_error_invalidFilePath);
        return;
    }
    emit imagesCommandResult(cmd ,cmd_state ,JKEnums::ImageCommandResult_NoError);
}

void ImageManager::separationScanDecode(const QStringList& fileList)
{
    QString str;
    bool found;
    foreach (QString filename, fileList) {
        str = decodeManager->decodeOneD(filename);
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
}

void ImageManager::separationScanDecodeEnd()
{
    QJsonObject jsonObj = QJsonDocument::fromJson(cmd_para.toLatin1()).object();
    int fileType = jsonObj.value("fileType").toInt(0);
    QString filePath = jsonObj.value("filePath").toString();
    QString tmpFile = filePath + "/tmp.tif";
    QString fullFileName;
    int i=0 ,j=0;
    foreach (struct Separation_data sd, separation_data) {
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
}

void ImageManager::decodeScanStart(const QString&)
{
}

void ImageManager::decodeScanDecode(const QStringList& fileList)
{
    QJsonObject jsonObj = QJsonDocument::fromJson(cmd_para.toLatin1()).object();
    int decodeType = jsonObj.value("codeType").toInt(0);
    struct DMDecodeResult dr;
    foreach (QString filename, fileList) {
        switch (decodeType) {
        case JKEnums::DecodeType_Qrcode:
            dr = decodeManager->decodeMultiQrcode(filename);
            break;
        case JKEnums::DecodeType_OneD:
            dr = decodeManager->decodeMultiOneD(filename);
            break;
        case JKEnums::DecodeType_All:
            dr = decodeManager->decodeMultiAll(filename);
            break;
        default:
            break;
        }
        if(!dr.fileName.isEmpty())
            decode_data << dr;
    }

}

void ImageManager::decodeScanEnd()
{
    QJsonObject jsonObj = QJsonDocument::fromJson(cmd_para.toLatin1()).object();
    QString fileName = jsonObj.value("fileName").toString();
    QString tmppath = getTempPath();
    fileName = tmppath + "/" + fileName;
    QFileInfo info(fileName);
    if(info.suffix() != "html")
        fileName += ".html";
    FILE* file = fopen(fileName.toLatin1().constData() ,"w");
    if(file == NULL)
        return;
    const char* cFileName;
    const char* cText;
    bool firstLine;

    fprintf(file ,"<html> <head> <title>QRCode/Barcode Detection Result</title> </head>\n");
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
                    QString type = result.text.contains("http://") ?"URI" :"TEXT";
                    cText  = result.text.toLatin1().constData();
//                    if(result.format != "RSS_14"){
                        if(type == "URI"){
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
}

void ImageManager::cmdExtra(int cmd ,QString para)
{
    bool success = true;
    QJsonObject jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
    QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
    switch (cmd) {
    case DeviceStruct::CMD_Cloud_isLogin:
    {
        bool isLogin = false;
        if(!cloudTypeText.compare("iCloud")){
            isLogin = iCloudCheckLogin(para);

        }
        jsonObj.insert("isLogin" ,isLogin);
    }
        cmdExtraResult(cmd ,QString(QJsonDocument(jsonObj).toJson()));
        break;
    case DeviceStruct::CMD_Cloud_getFileList:
        if(!cloudTypeText.compare("iCloud")){
            if(iCloudCheckLogin(para)){
                if(!iCloudGetFileList(para)){
                    if(!iCloudGetFileList(para)){
                        success = iCloudGetFileList(para);
                    }
                }
            }
        }
        jsonObj = QJsonDocument::fromJson(para.toLatin1()).object();
        jsonObj.insert("success" ,success);
        cmdExtraResult(cmd ,QString(QJsonDocument(jsonObj).toJson()));
        break;
    case DeviceStruct::CMD_Cloud_isExist:
    {
        bool isExist = false;
        if(!cloudTypeText.compare("iCloud")){
            QJsonObject selectList = jsonObj.value("selectList").toObject();
            foreach (QString key, selectList.keys()) {
                if(iCouldIsExist(selectList.value(key).toString())){
                    isExist = true;
                    break;
                }
            }
        }
        jsonObj.insert("isExist" ,isExist);
    }
        cmdExtraResult(cmd ,QString(QJsonDocument(jsonObj).toJson()));
        break;
    case DeviceStruct::CMD_Cloud_upload:
        if(!cloudTypeText.compare("iCloud")){
            QString str;
            QJsonObject selectList = jsonObj.value("selectList").toObject();
            foreach (QString key, selectList.keys()) {
                str = QString(QJsonDocument(jsonObj).toJson());
                success = iCloudUpload(selectList.value(key).toString());
                if(!success)
                    break;
                else{
                }
            }

            if(success && !iCloudGetFileList(str)){
                if(!iCloudGetFileList(str)){
                    iCloudGetFileList(str);
                }
            }
            jsonObj = QJsonDocument::fromJson(str.toLatin1()).object();
            jsonObj.insert("success" ,success);
        }
        cmdExtraResult(cmd ,QString(QJsonDocument(jsonObj).toJson()));
        break;
    default:
        break;
    }
}
