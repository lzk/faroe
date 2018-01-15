#include "jkinterface.h"
#include <QPrinterInfo>
#include <QQuickItem>

#include "../platform/devicemanager.h"
#include "ImageViewer/imagemodel.h"
#include "../imageManager/imagemanager.h"
using namespace JK;
#include "../platform/log.h"
JKInterface::JKInterface(QObject *parent)
    : QObject(parent)
{
    deviceManager = new DeviceManager(this);
    deviceManager->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,deviceManager ,SLOT(deleteLater()));

    connect(this ,SIGNAL(searchDeviceList()) ,deviceManager ,SLOT(searchDeviceList()));
//    connect(this ,SIGNAL(cancelSearch()) ,deviceManager ,SLOT(cancelSearchDeviceList()));
    connect(this ,SIGNAL(connectDevice(int)) ,deviceManager ,SLOT(connectDevice(int)));
    connect(deviceManager ,SIGNAL(searchComplete()) ,this , SIGNAL(searchComplete()));
    connect(deviceManager ,&DeviceManager::updateDeviceList ,this ,&JKInterface::updateDeviceList);
    connect(deviceManager ,&DeviceManager::deviceConnected ,this ,&JKInterface::deviceConnected);
    connect(deviceManager ,&DeviceManager::updateDeviceStatus ,this ,&JKInterface::updateDeviceStatus);

    connect(this ,&JKInterface::cmdToDevice ,deviceManager ,&DeviceManager::cmdToDevice);
    connect(deviceManager ,&DeviceManager::cmdResult ,this , &JKInterface::cmdResult);
    connect(deviceManager ,&DeviceManager::addImage ,this ,&JKInterface::addImage);
//    connect(deviceManager ,SIGNAL(progressChanged(qreal)) ,this , SLOT(setProgress(qreal)));

    imageManager = new ImageManager;
    imageManager->moveToThread(&thread);
    connect(this ,&JKInterface::imagesToPrint ,imageManager ,&ImageManager::imagesToPrint);
    connect(this ,&JKInterface::imagesToEmail ,imageManager ,&ImageManager::imagesToEmail);
    connect(this ,&JKInterface::imagesToFile ,imageManager ,&ImageManager::imagesToFile);
    connect(this ,&JKInterface::imagesToApplication ,imageManager ,&ImageManager::imagesToApplication);
    connect(this ,&JKInterface::imagesToFTP ,imageManager ,&ImageManager::imagesToFTP);
    connect(this ,&JKInterface::imagesToCloud ,imageManager ,&ImageManager::imagesToCloud);

    thread.start();
    deviceManager->initAfterMoveToThread();
}

JKInterface::~JKInterface()
{
    thread.quit();
    thread.wait();
}
void JKInterface::setScanDataHandle(QObject *scanData)
{
    this->scanData = scanData;
}

QStringList JKInterface::getPrinterName()
{
    return QPrinterInfo::availablePrinterNames();
}


void JKInterface::updateDeviceList(QStringList deviceList)
{
    scanData->setProperty("model_deviceList" ,deviceList);

}

void JKInterface::deviceConnected(QString currentDevice)
{
    scanData->setProperty("currentDevice" ,currentDevice);
}

QString JKInterface::getCurrentDevice()
{
    QString str = scanData->property("currentDevice").toString();
    qDebug()<<"current device:"<<str;
    return str;
}

void JKInterface::updateDeviceStatus(bool status)
{
    scanData->setProperty("deviceStatus" ,status);
}

void JKInterface::addImage(QString filename,QSize sourceSize)
{
    imageModel->addImage(ImageItem(filename ,sourceSize));
}

void JKInterface::cancelScan()
{
    deviceManager->cancelScan();
}

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
void JKInterface::toPrint(QList<int> selectedList)
{
    if(selectedList.length()<=0){
        return;
    }
    QPrintDialog printDialog;
    if (printDialog.exec() == QDialog::Accepted) {
        emit imagesToPrint(imageModel->getFileList(selectedList) ,printDialog.printer()->printerName());
    }
}

void JKInterface::toEmail(QList<int> selectedList ,int fileType)
{
    if(selectedList.length()<=0){
        return;
    }
    emit imagesToEmail(imageModel->getFileList(selectedList) ,fileType);
}

void JKInterface::toFile(QList<int> selectedList ,const QString& filePath)
{
    if(selectedList.length()<=0){
        return;
    }
    emit imagesToFile(imageModel->getFileList(selectedList) ,filePath);
}

void JKInterface::toFTP(QList<int> selectedList ,const QString& jsonData)
{
    if(selectedList.length()<=0){
        return;
    }
    emit imagesToFTP(imageModel->getFileList(selectedList) ,jsonData);
}

void JKInterface::toApplication(QList<int> selectedList , const QString& fileName)
{
    if(selectedList.length()<=0){
        return;
    }
    emit imagesToApplication(imageModel->getFileList(selectedList) ,fileName);
}

void JKInterface::toCloud(QList<int> selectedList ,const QString& jsonData)
{
    if(selectedList.length()<=0){
        return;
    }
    emit imagesToCloud(imageModel->getFileList(selectedList) ,jsonData);
}
