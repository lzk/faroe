#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QSize>
#include <QList>
#include <QTimer>
#include <QJsonObject>
#include "../lld/device.h"
#include "netio.h"
#include "appqt.h"
#include "../lld/setter.h"
#ifdef Q_OS_MAC
#include "mac/usbio.h"
#endif
using JK::Device;
using JK::DeviceInfo;

enum{
    DEVICE_NODEVICE,
    DEVICE_USB,
    DEVICE_NET,
    DEVICE_OTHER
};

class JKInterface;
class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();
    int isCancelSearch();
    void addDeviceInfo(DeviceInfo* ,int);
    static void addDevice(DeviceInfo* ,void*);
    static void addUsbDevice(DeviceInfo* ,void*);
    void cancelScan(bool cancel);

signals:
    void progressChanged(int progress ,int page);
    void updateDeviceList(QStringList);
    void scanResult(int);
    void cmdResult(int cmd ,int err ,QString);
    void searchComplete();
    void deviceConnected(QString);
    void updateDeviceStatus(bool);
    void scanedImage(QString ,QSize);

public slots:
    void searchDeviceList();
    void cancelSearchDeviceList();
    void connectDevice(int);

    void cmdToDevice(int ,QString);
    void init();

private slots:
    void watchDevice();
    void addImage(QString);

private:
    JKInterface* jkInterface;
    Device* device;
    int cancelSearch;
    int currentCmd;
    QStringList m_deviceList;
    QString currentDeviceName;
    QList<DeviceInfo> deviceList;
    NetIO netIO;
#ifdef Q_OS_MAC
    UsbIO usbIO;
#endif
    AppQt platformApp;
#if TEST
    void testAddImage(const QString&);
#endif

    Scanner::Setting parseUiScannerSetting(const QString&);
    Setter::struct_wifiSetting parseUiWifiSetting(const QString&);
    QString parseUiPassword(const QString&);
    int parseUiSaveTime(const QString&);
    void uiParseSaveTime(QJsonObject& ,int);
    int parseUiOffTime(const QString&);
    void uiParseOffTime(QJsonObject& ,int);
    Setter::struct_deviceSetting parseUiDeviceSetting(const QString&);
    void uiParseDeviceSetting(QJsonObject& ,Scanner::struct_deviceSetting);
    void uiParseWifiInfo(QJsonObject& ,Setter::struct_wifiInfo);
    void uiParseIpv4(QJsonObject& ,Setter::struct_ipv4);
    Setter::struct_ipv4 parseUiIpv4(const QString&);
    void uiParseSoftap(QJsonObject& ,Setter::struct_softAp);
    Setter::struct_softAp parseUiSoftap(const QString&);
    void uiParsePowerSupply(QJsonObject& ,int);
    Scanner::struct_deviceSetting parseUiPowerSaveTime(const QString&);

    void connectDeviceInfo(DeviceInfo*);
};

#endif // DEVICEMANAGER_H
