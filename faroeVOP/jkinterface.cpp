#include "jkinterface.h"
#include "../lld/device.h"
using namespace JK;
#include <QDebug>
const UI_Scanner_Settings defaultScannerSetting = {
    .ADFMode=true,
    .dpi = 1,
    .colorMode = 0,
    .scanAreaSize = 0,
    .MultiFeed = true,
    .contrast = 50,
    .brightness = 50,
    .AutoCropAndDeskew = true
};
JKInterface::JKInterface(QObject *parent) : QObject(parent)
  ,m_progress(0.0)
{
    memset(&uiSettings ,0 ,sizeof(uiSettings));
    uiSettings.scannerSettings = defaultScannerSetting;
    scannerSettings = defaultScannerSetting;

    deviceManager = new DeviceManager(this);
    deviceManager->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,deviceManager ,SLOT(deleteLater()));
    connect(deviceManager ,SIGNAL(progressChanged(qreal)) ,this , SLOT(setProgress(qreal)));
    connect(deviceManager ,SIGNAL(addImage(QString,QSize)) ,this ,SLOT(addImage(QString,QSize)));
    connect(deviceManager ,SIGNAL(scanResult(int)) ,this , SIGNAL(scanResult(int)));
    connect(this ,SIGNAL(scan()) ,deviceManager ,SLOT(scan()));
    connect(this ,SIGNAL(deviceChanged(QString)) ,deviceManager ,SLOT(resolveUrl(QString)));
    thread.start();
}


JKInterface::~JKInterface()
{
    thread.quit();
    thread.wait();
}


void JKInterface::cancelScan()
{
    if(deviceManager)
        deviceManager->cancelScan();
}

void JKInterface::addImage(QString filename ,QSize sourceSize)
{
    model.addImage(ImageItem(filename ,sourceSize));
}

Scanner::Setting JKInterface::parseUiScannerSetting()
{
    Scanner::Setting setting;
    int dpi,width,height,x;
    switch(scannerSettings.dpi){
    case 0:dpi = 200;break;
    case 1:dpi = 300;break;
    case 2:dpi = 600;break;
    default:dpi=300;break;
    }
    switch (scannerSettings.scanAreaSize) {
    case 1://A4
        width=(2480*dpi/300 + 7)/8;
        height=3512*dpi/300;
        x=35*dpi/300;
        break;
    case 2://Letter
        width=(2536*dpi/300+7)/8;
        height=3296*dpi/300;
        x=9*dpi/300;
        break;
    case 0://auto
    default:
        width=(2592*dpi/300+7)/8;
        height=32400*dpi/300;
        x=0;
        break;
    }
    QMutexLocker locker(&mutex);
    setting.BitsPerPixel = scannerSettings.colorMode = 0 ?IMG_24_BIT :IMG_8_BIT;
    setting.resolution = dpi;
    setting.width = width;
    setting.height = height;
    setting.x = x;
    setting.y = 0;

    setting.contrast = scannerSettings.contrast;
    setting.brightness = scannerSettings.brightness;
    setting.ADFMode = scannerSettings.ADFMode? SCAN_AB_SIDE : SCAN_A_SIDE;
    setting.MultiFeed = scannerSettings.MultiFeed;
    setting.AutoCrop = scannerSettings.AutoCropAndDeskew;
    setting.onepage = SCAN_PAGE;
    setting.source = SCAN_SOURCE;
    setting.format = IMG_FORMAT;
    return setting;
}
Setter::struct_wifiSetting JKInterface::parseUiWifiSetting()
{
    QMutexLocker locker(&mutex);
}

QString JKInterface::parseUiPassword()
{
    QMutexLocker locker(&mutex);
}

int JKInterface::parseUiSaveTime()
{
    QMutexLocker locker(&mutex);
}

void JKInterface::uiParseSaveTime(int)
{
    QMutexLocker locker(&mutex);
}

void JKInterface::uiParseWifiInfo(Setter::struct_wifiInfo)
{
    QMutexLocker locker(&mutex);
}

void JKInterface::uiParseIpv4(Setter::struct_ipv4)
{
    QMutexLocker locker(&mutex);
}

Setter::struct_ipv4 JKInterface::parseUiIpv4()
{
    QMutexLocker locker(&mutex);
}

void JKInterface::uiParseSoftap(Setter::struct_softAp)
{
    QMutexLocker locker(&mutex);
}

Setter::struct_softAp JKInterface::parseUiSoftap()
{
    QMutexLocker locker(&mutex);
}

ImageModel* JKInterface::getImageModel()
{
    return &model;
}

qreal JKInterface::getProgress()
{
    return m_progress;
}

void JKInterface::setProgress(qreal prgrs)
{
    m_progress = prgrs;
    emit progressChanged();
}


bool JKInterface::ADFMode()
{
    return uiSettings.scannerSettings.ADFMode;
}

int JKInterface::dpi()
{
    return uiSettings.scannerSettings.dpi;
}
int JKInterface::colorMode()
{
    return uiSettings.scannerSettings.colorMode;
}
int JKInterface::scanAreaSize()
{
    return uiSettings.scannerSettings.scanAreaSize;
}
bool JKInterface::MultiFeed()
{
    return uiSettings.scannerSettings.MultiFeed;
}
int JKInterface::contrast()
{
    return uiSettings.scannerSettings.contrast;
}
int JKInterface::brightness()
{
    return uiSettings.scannerSettings.brightness;
}
bool JKInterface::AutoCropAndDeskew()
{
    return uiSettings.scannerSettings.AutoCropAndDeskew;
}

void JKInterface::setADFMode(bool value)
{
    uiSettings.scannerSettings.ADFMode = value;
    emit ADFModeChanged();
}

void JKInterface::setDpi(int value)
{
    uiSettings.scannerSettings.dpi = value;
    emit dpiChanged();
}
void JKInterface::setColorMode(int value)
{
    uiSettings.scannerSettings.colorMode = value;
    emit colorModeChanged();
}
void JKInterface::setScanAreaSize(int value)
{
    uiSettings.scannerSettings.scanAreaSize = value;
    emit scanAreaSizeChanged();
}
void JKInterface::setMultiFeed(bool value)
{
    uiSettings.scannerSettings.MultiFeed = value;
    emit MultiFeedChanged();
}
void JKInterface::setContrast(int value)
{
    uiSettings.scannerSettings.contrast = value;
    emit contrastChanged();
}
void JKInterface::setBrightness(int value)
{
    uiSettings.scannerSettings.brightness = value;
    emit brightnessChanged();
}
void JKInterface::setAutoCropAndDeskew(bool value)
{
    uiSettings.scannerSettings.AutoCropAndDeskew = value;
    emit AutoCropAndDeskewChanged();
}
 void JKInterface::setDefault()
 {
     uiSettings.scannerSettings = defaultScannerSetting;
     emit ADFModeChanged();
     emit dpiChanged();
     emit colorModeChanged();
     emit scanAreaSizeChanged();
     emit MultiFeedChanged();
     emit contrastChanged();
     emit brightnessChanged();
     emit AutoCropAndDeskewChanged();
 }
 void JKInterface::setOK()
 {
     scannerSettings = uiSettings.scannerSettings;
 }
 void JKInterface::loadScannerSettings()
 {
     uiSettings.scannerSettings = scannerSettings;
 }

