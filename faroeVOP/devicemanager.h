#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QSize>
#include "../lld/device.h"
#include "netio.h"
#include "appqt.h"
using JK::Device;

enum{
    DEVICE_NODEVICE,
    DEVICE_USB,
    DEVICE_NET,
    DEVICE_OTHER
};

struct DeviceInfo
{
    int type;
    QString name;
    QString hostName;
    QString ipAddress;
    int pidvid;
};

class JKInterface;
class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();

signals:
    void progressChanged(qreal);
    void addImage(QString  ,QSize);
    void scanResult(int);

public slots:
    void scan();
    void cancelScan();
    void resolveUrl(const QString& url);

private:
    JKInterface* jkInterface;
    Device* device;
    NetIO netIO;
    AppQt platformApp;
    void saveImage(const QImage& image);
};

#endif // DEVICEMANAGER_H
