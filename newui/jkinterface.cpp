#include "jkinterface.h"
#include <QPrinterInfo>
#include <QQuickItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDateTime>
#include <QPrintDialog>
#include <QPrinter>

#include "../platform/devicemanager.h"
#include "ImageViewer/imagemodel.h"
using namespace JK;
#include "../platform/devicestruct.h"
#include "../newui/jkenums.h"
#include "../platform/platform.h"
#include "../platform/log.h"

#include "worker.h"
JKInterface::JKInterface(QObject *parent)
    : QObject(parent)
{
    platform_init();

    worker = new Worker(this);
    worker->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,worker ,SLOT(deleteLater()));

    thread.start();
}

JKInterface::~JKInterface()
{
    thread.quit();
    thread.wait();
    this->scanData = NULL;
}
void JKInterface::setScanDataHandle(QObject *scanData)
{
    this->scanData = scanData;
}

bool checkWifiStatus();
bool JKInterface::getWifiStatus()
{
    bool isEnabled = false;
    isEnabled = checkWifiStatus();
    return isEnabled;
}

QStringList JKInterface::getPrinterName()
{
    QStringList sl;
    printerNameList = QStringList();
    foreach (QPrinterInfo pi, QPrinterInfo::availablePrinters()) {
//        qDebug()<<pi.printerName();
//        qDebug()<<pi.makeAndModel();
//        qDebug()<<pi.description();
        printerNameList << pi.printerName();
        sl << pi.description();
    }
    return sl;
//    return QPrinterInfo::availablePrinterNames();
}
QString JKInterface::getDefaultPrinterName()
{
    QList<QPrinterInfo> pis = QPrinterInfo::availablePrinters();
    if(pis.isEmpty())
        return QString();
    QString defaultPrinter = pis.first().description();
    foreach (QPrinterInfo pi, pis) {
        if(pi.isDefault()){
            defaultPrinter = pi.description();
            break;
        }
    }
    return defaultPrinter;
}
void JKInterface::updateDeviceList(QStringList deviceList)
{
    if(!qobject_cast<QObject*>(scanData))
        return;
    scanData->setProperty("model_deviceList" ,deviceList);
}

void JKInterface::deviceConnected(QString currentDevice)
{
    if(!qobject_cast<QObject*>(scanData))
        return;
    scanData->setProperty("currentDevice" ,currentDevice);
    emit deviceConnectCompleted();
}

QString JKInterface::getCurrentDevice()
{
    if(!qobject_cast<QObject*>(scanData))
        return "";
    QString str = scanData->property("currentDevice").toString();
    return str;
}

void JKInterface::updateDeviceStatus(bool status)
{
    if(!qobject_cast<QObject*>(scanData))
        return;
    scanData->setProperty("deviceStatus" ,status);
}

void JKInterface::setCmd(int cmd ,const QString& data,QStringList fileList)
{
    qDebug()<<"cmd:"<<cmd;
    qDebug()<<"cmd para"<<data;

    cancel(false);
    switch (cmd) {
    //quick scan and decode sepearation check first.
    case DeviceStruct::CMD_QuickScan_ToFile:{
        QJsonObject jsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
        QString filePath = jsonObj.value("filePath").toString();
        if(!QDir(filePath).exists()){
            emit cmdResultToUi(cmd ,data ,JKEnums::ImageCommandResult_error_invalidFilePath);
            return;
        }
        QString fileName_ = jsonObj.value("fileName").toString();
        QString fullPath = filePath + "/" + fileName_  + "_" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
        int fileType = jsonObj.value("fileType").toInt(0);
        switch (fileType) {
        case 0:     fullPath += ".pdf";    break;
        case 1:     fullPath += ".tif";    break;
        case 2:     fullPath += ".jpg";    break;
        case 3:     fullPath += ".bmp";    break;
        default:
            break;
        }
        jsonObj.insert("fullFileName" ,fullPath);
        cmdToWorker(cmd ,QString(QJsonDocument(jsonObj).toJson()));
    }
        break;
    case DeviceStruct::CMD_QuickScan_ToPrint:
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
        QString printerName = jsonObj.value("printerName").toString();
        foreach (QPrinterInfo pi, QPrinterInfo::availablePrinters()) {
            if(!printerName.compare(pi.description())){
                printerName = pi.printerName();
            }
        }
        QStringList printerList = QPrinterInfo::availablePrinterNames();
        if(printerList.isEmpty() || !printerList.contains(printerName)){
            emit cmdResultToUi(cmd ,data ,JKEnums::ImageCommandResult_error_invalidPrinter);
            return;
        }
        jsonObj.remove("printerName");
        jsonObj.insert("printerName" ,printerName);
        cmdToWorker(cmd ,QString(QJsonDocument(jsonObj).toJson()));
    }
        break;
    case DeviceStruct::CMD_ScanTo_ToPrint:{
        if(QPrinterInfo::availablePrinterNames().isEmpty()){
            emit cmdResultToUi(cmd ,data ,JKEnums::ImageCommandResult_error_invalidPrinter);
            break;
        }
        QPrintDialog printDialog;
        if (printDialog.exec() == QDialog::Accepted){
            QPrinter* printer = printDialog.printer();

            QJsonObject obj{
                {"printerName",printer->printerName()}
            };
            cmdToWorker(cmd ,QString(QJsonDocument(obj).toJson()) ,fileList);

        }
    }
        break;
    case DeviceStruct::CMD_ScanTo_ToFile:
    case DeviceStruct::CMD_ScanTo_ToEmail:
    case DeviceStruct::CMD_ScanTo_ToApplication:
    case DeviceStruct::CMD_ScanTo_ToFTP:
    case DeviceStruct::CMD_ScanTo_ToCloud:
        cmdToWorker(cmd ,data ,fileList);
        break;
    case DeviceStruct::CMD_DecodeScan:
    case DeviceStruct::CMD_SeperationScan:
    case DeviceStruct::CMD_QuickScan:
    case DeviceStruct::CMD_QuickScan_ToApplication:
    case DeviceStruct::CMD_QuickScan_ToCloud:
    case DeviceStruct::CMD_QuickScan_ToEmail:
    case DeviceStruct::CMD_QuickScan_ToFTP:
    case DeviceStruct::CMD_ScanTo:
    default:
        cmdToWorker(cmd ,data);
        break;
    }
}

void JKInterface::scanedImage(QString filename,QSize sourceSize)
{
    imageModel->addImage(ImageItem(filename ,sourceSize));
}


void JKInterface::cancelScan()
{
//    deviceManager->cancelScan(true);
//    imageManager->cancel(true);
    cancel(true);
}

void JKInterface::cancelSearch()
{
    cancel(true);
}

void JKInterface::cancelImageHandle()
{
//    imageManager->cancel(true);
    cancel(true);
}

QString JKInterface::homeDictory()
{
    return QDir::home().absolutePath();
}

bool JKInterface::pathExist(const QString& filePath)
{
    return QDir(filePath).exists();
}

void JKInterface::showMinimize(QWindow* window)
{
    windowShowMinimize(window);
}

void JKInterface::setWindowFrameless(QWindow* window)
{
    windowSetWindowFrameless(window);
}

#include <QSysInfo>
int JKInterface::macVersion()
{
    int version = QSysInfo::macVersion();
    LOG_PARA("Version:%d" ,version);
    return version;
}

bool JKInterface::validate(const QValidator* validator ,QString& text)
{
    int pos;
    return validator->validate(text ,pos) != QValidator::Invalid;
}

#include <QHostAddress>
bool JKInterface::isSubmask(const QString& submask)
{
    QHostAddress address(submask);
    int ipv4 = address.toIPv4Address();
    if(ipv4 == 0)
        return true;
    int index = 0;
    for(;index < 32 ;index++){
        if(0 != (ipv4 & (0x80000000 >> index)))
            continue;
        else
            break;
    }
    if(index == 32)
        return false;
    for(;index < 32 ;index ++){
        if(0 == (ipv4 & (0x80000000 >> index)))
            continue;
        else
            break;
    }
    return index == 32;
}

bool JKInterface::isIpv4(const QString& ip)
{
    QHostAddress address(ip);
    int ipv4 = address.toIPv4Address();
    int val0 = (ipv4 >> 24);
    return val0 != 127 && val0 < 224;
}
