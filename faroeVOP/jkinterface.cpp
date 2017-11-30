#include "jkinterface.h"
#include "../lld/device.h"
using namespace JK;
#include <QDebug>
#include "../platform/log.h"
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
    init_log();
    memset(&uiSettings ,0 ,sizeof(uiSettings));
    uiSettings.scannerSettings = defaultScannerSetting;
    scannerSettings = defaultScannerSetting;

    deviceManager = new DeviceManager(this);
    deviceManager->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,deviceManager ,SLOT(deleteLater()));
    connect(deviceManager ,SIGNAL(progressChanged(qreal)) ,this , SLOT(setProgress(qreal)));
    connect(deviceManager ,SIGNAL(addImage(QString,QSize)) ,this ,SLOT(addImage(QString,QSize)));
    connect(deviceManager ,SIGNAL(addDeviceInfo(QString)) ,this ,SLOT(addDeviceInfo(QString)));
    connect(deviceManager ,SIGNAL(cmdResult(int,int)) ,this , SIGNAL(cmdResult(int,int)));
    connect(deviceManager ,SIGNAL(searchComplete()) ,this , SIGNAL(searchComplete()));
    connect(this ,SIGNAL(connectDevice(int)) ,deviceManager ,SLOT(connectDevice(int)));
    connect(this ,SIGNAL(searchDeviceList()) ,deviceManager ,SLOT(searchDeviceList()));
    connect(this ,SIGNAL(cancelSearch()) ,deviceManager ,SLOT(cancelSearchDeviceList()));
    connect(this ,SIGNAL(deviceCmd(int)) ,deviceManager ,SLOT(deviceCmd(int)));
    thread.start();
}

JKInterface::~JKInterface()
{
    thread.quit();
    thread.wait();
}

void JKInterface::log(const QString& msg)
{
    logout(msg);
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
    QMutexLocker locker(&mutex);
    setting.BitsPerPixel = scannerSettings.colorMode == 0 ?SETTING_IMG_24_BIT :SETTING_IMG_8_BIT;
    switch(scannerSettings.dpi){
    case 0:setting.dpi_x = setting.dpi_y = 200;break;
    case 1:setting.dpi_x = setting.dpi_y = 300;break;
    case 2:setting.dpi_x = setting.dpi_y = 600;break;
    default:setting.dpi_x = setting.dpi_y = 300;break;
    }
    switch (scannerSettings.scanAreaSize) {
    case 1://A4
        setting.scan_doc_size = SETTING_DOC_SIZE_A4;
        break;
    case 2://Letter
        setting.scan_doc_size = SETTING_DOC_SIZE_LT;
        break;
    case 0://auto
    default:
        setting.scan_doc_size = SETTING_DOC_SIZE_FULL;
        break;
    }

    setting.contrast = scannerSettings.contrast;
    setting.brightness = scannerSettings.brightness;
    setting.ADFMode = scannerSettings.ADFMode? SETTING_SCAN_AB_SIDE : SETTING_SCAN_A_SIDE;
    setting.MultiFeed = scannerSettings.MultiFeed;
    setting.AutoCrop = scannerSettings.AutoCropAndDeskew;
    setting.onepage = SETTING_SCAN_PAGE;
    setting.source = SETTING_SCAN_ADF;
    setting.format = SETTING_IMG_FORMAT_JPG;
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

 void JKInterface::addDeviceInfo(QString str)
 {
     m_deviceList.addString( str);
//     m_deviceList << str;
 }

#include <QPrintDialog>
void JKInterface::test()
{
    QPrintDialog dialog;
    dialog.exec();
    dialog.move(0 ,1000);
}
