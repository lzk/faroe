#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QSize>
#include <QList>
#include "../lld/device.h"
#include "netio.h"
#include "mac/usbio.h"
#include "appqt.h"
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

signals:
    void progressChanged(qreal);
    void addImage(QString  ,QSize);
    void addDeviceInfo(QString);
    void scanResult(int);
    void cmdResult(int cmd ,int err);

public slots:
    void scan();
    void cancelScan();
    void searchDeviceList();
    void cancelSearchDeviceList();
    void connectDevice(int);
    void deviceCmd(int cmd);

private:
    JKInterface* jkInterface;
    Device* device;
    int cancelSearch;
    QList<DeviceInfo> deviceList;
    NetIO netIO;
    UsbIO usbIO;
    AppQt platformApp;
    void saveImage(const QImage& image);
};

#endif // DEVICEMANAGER_H
