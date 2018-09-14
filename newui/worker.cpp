#include "worker.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../platform/platform.h"
#include "jkinterface.h"
#include "../platform/devicestruct.h"
#include "../platform/devicemanager.h"
#include "../functions/imagefunctions.h"

Worker::Worker(QObject *parent)
    : jkInterface(static_cast<JKInterface*>(parent))
    ,imageFunctions(nullptr)
    ,cmd_status(0)
    ,err(JKEnums::CommandResult_NoError)
{
    deviceManager = new DeviceManager(this);
    deviceManager->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,deviceManager ,SLOT(deleteLater()));

    connect(jkInterface ,&JKInterface::cmdToWorker ,this ,&Worker::cmdFromUi);
    connect(this ,&Worker::cmdResultToUi ,jkInterface ,&JKInterface::cmdResultToUi);
    connect(jkInterface ,&JKInterface::cancel ,this ,&Worker::cancel ,Qt::DirectConnection);

    connect(jkInterface ,&JKInterface::init ,deviceManager ,&DeviceManager::init);
    connect(this ,&Worker::cmdToDevice ,deviceManager ,&DeviceManager::cmdToDevice);
    connect(deviceManager ,&DeviceManager::cmdResult ,this , &Worker::deviceCmdResult);
    connect(deviceManager ,&DeviceManager::cmdResult ,this , &Worker::deviceCmdResult_immediately ,Qt::DirectConnection);
    connect(deviceManager ,&DeviceManager::scanedImage ,this ,&Worker::scanedImage);
    connect(deviceManager ,&DeviceManager::progressChanged ,jkInterface ,&JKInterface::progressChanged);

    connect(deviceManager ,&DeviceManager::scanedImage ,jkInterface ,&JKInterface::scanedImage);

    connect(jkInterface ,SIGNAL(searchDeviceList()) ,deviceManager ,SLOT(searchDeviceList()));
    connect(jkInterface ,SIGNAL(connectDevice(int)) ,deviceManager ,SLOT(connectDevice(int)));
    connect(deviceManager ,SIGNAL(searchComplete()) ,jkInterface , SIGNAL(searchComplete()));
    connect(deviceManager ,&DeviceManager::updateDeviceList ,jkInterface ,&JKInterface::updateDeviceList);
    connect(deviceManager ,&DeviceManager::deviceConnected ,jkInterface ,&JKInterface::deviceConnected);
    connect(deviceManager ,&DeviceManager::updateDeviceStatus ,jkInterface ,&JKInterface::updateDeviceStatus);
    thread.start();
}

Worker::~Worker()
{
    thread.quit();
    thread.wait();
    if(imageFunctions)
        delete imageFunctions;
}

void Worker::cancel(bool ccl)
{
    QMutexLocker locker(&mutex);
    if(imageFunctions)
        imageFunctions->cancel(ccl);
    deviceManager->cancelScan(ccl);
    if(ccl){
        deviceManager->cancelSearchDeviceList();
    }
}

void Worker::cmdFromUi(int cmd ,QString data ,QStringList fileList)
{
    if(cmd_status)
        return;
    cmd_status = 1;
    this->cmd = cmd;
    this->para = data;
    err = JKEnums::CommandResult_NoError;
    if(imageFunctions){
        delete imageFunctions;
        imageFunctions = nullptr;
    }
    switch (cmd) {
    case DeviceStruct::CMD_DecodeScan:
        imageFunctions = new ImageFunctions_Decode();
        break;
    case DeviceStruct::CMD_SeperationScan:
        imageFunctions = new ImageFunctions_Separation();
        break;
    case DeviceStruct::CMD_QuickScan_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToPrint:
        imageFunctions = new ImageFunctions_ToPrint();
        break;
    case DeviceStruct::CMD_QuickScan_ToFile:
    case DeviceStruct::CMD_ScanTo_ToFile:
        imageFunctions = new ImageFunctions_ToFile(cmd ==  DeviceStruct::CMD_QuickScan_ToFile);
        break;
    case DeviceStruct::CMD_QuickScan_ToEmail:
    case DeviceStruct::CMD_ScanTo_ToEmail:
        imageFunctions = new ImageFunctions_ToEmail();
        break;
    case DeviceStruct::CMD_QuickScan_ToApplication:
    case DeviceStruct::CMD_ScanTo_ToApplication:
        imageFunctions = new ImageFunctions_ToApplication();
        break;
    case DeviceStruct::CMD_QuickScan_ToCloud:
    case DeviceStruct::CMD_ScanTo_ToCloud:
        imageFunctions = new ImageFunctions_ToCloud();
        break;
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_ScanTo_ToFTP:
        imageFunctions = new ImageFunctions_ToFtp();
    default:
        break;
    }
    cancel(false);
    switch (cmd) {
    case DeviceStruct::CMD_DecodeScan:
    case DeviceStruct::CMD_SeperationScan:
    case DeviceStruct::CMD_QuickScan_ToPrint:
    case DeviceStruct::CMD_QuickScan_ToFile:
    case DeviceStruct::CMD_QuickScan_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToApplication:
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_QuickScan_ToCloud:
        if(imageFunctions){
            err = imageFunctions->preFunction(data);
            if(err == JKEnums::CommandResult_NoError){
                emit cmdToDevice(DeviceStruct::CMD_SCAN ,data);
                emit cmdResultToUi(cmd ,data ,0 ,0 ,JKEnums::CommandPhase_start);
            }
        }
        break;
    case DeviceStruct::CMD_ScanTo_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToFile:
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_ScanTo_ToApplication:
    case DeviceStruct::CMD_ScanTo_ToCloud:
    case DeviceStruct::CMD_ScanTo_ToFTP:
        if(imageFunctions){
            emit cmdResultToUi(cmd ,data ,0 ,0 ,JKEnums::CommandPhase_start);
            emit cmdResultToUi(cmd ,data ,0 ,0 ,JKEnums::CommandPhase_processing);
            err = imageFunctions->preFunction(data);
            if(err){
                cmdComplete(cmd ,data ,err ,JKEnums::ResultType_common);
                break;
            }
            foreach (QString filename, fileList) {
                err = imageFunctions->function(QStringList()<<filename ,data);
                if(err){
                    break;
                }
            }
            if(err){
                cmdComplete(cmd ,data ,err ,JKEnums::ResultType_common);
                break;
            }
            err = imageFunctions->postFunction(data);
            cmdComplete(cmd ,data ,err ,JKEnums::ResultType_common);
        }
        break;
    case DeviceStruct::CMD_ScanTo:
        emit cmdToDevice(DeviceStruct::CMD_ScanTo ,data);
        emit cmdResultToUi(cmd ,data ,0 ,0 ,JKEnums::CommandPhase_start);
        break;
    case DeviceStruct::CMD_Cloud_isLogin:
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
        QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
        bool isLogin = false;
        if(!cloudTypeText.compare("iCloud")){
            isLogin = iCloudCheckLogin(para);

        }
        jsonObj.insert("isLogin" ,isLogin);
        cmdComplete(cmd ,QString(QJsonDocument(jsonObj).toJson()));
    }
        break;
    case DeviceStruct::CMD_Cloud_getFileList:{
        bool success = true;
        QJsonObject jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
        QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
        if(!cloudTypeText.compare("iCloud")){
            if(iCloudCheckLogin(para)){
                if(!iCloudGetFileList(para)){
                    if(!iCloudGetFileList(para)){
                        success = iCloudGetFileList(para);
                    }
                }
            }
        }
        jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
        jsonObj.insert("success" ,success);
        cmdComplete(cmd ,QString(QJsonDocument(jsonObj).toJson()));
    }
        break;
    case DeviceStruct::CMD_Cloud_isExist:
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
        QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
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
        cmdComplete(cmd ,QString(QJsonDocument(jsonObj).toJson()));
    }
        break;
    case DeviceStruct::CMD_Cloud_upload:{
        bool success = true;
        QJsonObject jsonObj = QJsonDocument::fromJson(para.toUtf8()).object();
        QString cloudTypeText = jsonObj.value("cloudTypeText").toString();
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
            jsonObj = QJsonDocument::fromJson(str.toUtf8()).object();
            jsonObj.insert("success" ,success);
        }
        cmdComplete(cmd ,QString(QJsonDocument(jsonObj).toJson()));
    }
        break;
    default:
        emit cmdToDevice(cmd ,data);
        emit cmdResultToUi(cmd ,data ,0 ,0 ,JKEnums::CommandPhase_start);
        break;
    }
}

void Worker::deviceCmdResult_immediately(int /*cmd*/,int ,QString data)
{
//    if(cmd == DeviceStruct::CMD_SCAN)
        emit cmdResultToUi(this->cmd ,data ,0 ,0 ,JKEnums::CommandPhase_processing);
}

void Worker::deviceCmdResult(int,int result ,QString data)
{
    switch (cmd) {
    case DeviceStruct::CMD_DecodeScan:
    case DeviceStruct::CMD_SeperationScan:
    case DeviceStruct::CMD_QuickScan_ToPrint:
    case DeviceStruct::CMD_QuickScan_ToFile:
    case DeviceStruct::CMD_QuickScan_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToApplication:
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_QuickScan_ToCloud:
        if(err != JKEnums::CommandResult_NoError){
            cmdComplete(cmd ,data ,err ,JKEnums::ResultType_common);
            return;
        }
        if(result){
            qDebug()<<"scanning err:"<<result;
            cmdComplete(cmd ,data ,result ,JKEnums::ResultType_scanning);
            return;
        }
        if(imageFunctions){
            err = imageFunctions->postFunction(para);
            cmdComplete(cmd ,data ,err ,JKEnums::ResultType_common);
        }
        break;
    case DeviceStruct::CMD_ScanTo_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToFile:
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_ScanTo_ToApplication:
    case DeviceStruct::CMD_ScanTo_ToCloud:
    case DeviceStruct::CMD_ScanTo_ToFTP:
        break;
    default:
        cmdComplete(cmd ,data ,result ,JKEnums::ResultType_common);
        break;
    }
}

void Worker::scanedImage(QString filename,QSize)
{
    if(err != JKEnums::CommandResult_NoError){
        return;
    }
    switch (cmd) {
    case DeviceStruct::CMD_DecodeScan:
    case DeviceStruct::CMD_SeperationScan:
    case DeviceStruct::CMD_QuickScan_ToPrint:
    case DeviceStruct::CMD_QuickScan_ToFile:
    case DeviceStruct::CMD_QuickScan_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToApplication:
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_QuickScan_ToCloud:
        if(imageFunctions){
            err = imageFunctions->function(QStringList()<<filename ,para);
        }
        break;
    case DeviceStruct::CMD_ScanTo_ToPrint:
    case DeviceStruct::CMD_ScanTo_ToFile:
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_ScanTo_ToApplication:
    case DeviceStruct::CMD_ScanTo_ToCloud:
    case DeviceStruct::CMD_ScanTo_ToFTP:
        break;
    default:
        break;
    }
}

void Worker::cmdComplete(int cmd ,QString data ,int result ,int resultType)
{
    emit cmdResultToUi(cmd ,data ,result ,resultType);
    cmd_status = 0;
    if(imageFunctions){
        delete imageFunctions;
        imageFunctions = nullptr;
    }
}
