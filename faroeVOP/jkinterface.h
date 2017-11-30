#ifndef JKINTERFACE_H
#define JKINTERFACE_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QImage>
#include "../thumbnailviewer/imagemodel.h"
#include "../platform/devicemanager.h"
#include "stringmodel.h"
#include "../lld/scanner.h"
#include "../lld/setter.h"
typedef struct{
    bool ADFMode;
    int dpi;
    int colorMode;
    int scanAreaSize;
    bool MultiFeed;
    int contrast;
    int brightness;
    bool AutoCropAndDeskew;
}
    UI_Scanner_Settings;
typedef struct {
    UI_Scanner_Settings scannerSettings;
}
    UI_Settings;
struct Setting;
class JKInterface : public QObject
{
    Q_OBJECT
public:
    explicit JKInterface(QObject *parent = nullptr);
    ~JKInterface();

    Q_PROPERTY(StringModel* deviceList READ getDeviceList CONSTANT)
//    Q_PROPERTY(QStringList deviceList READ getDeviceList CONSTANT)
    Q_PROPERTY(ImageModel* model READ getImageModel CONSTANT)
    Q_PROPERTY(qreal progress READ getProgress NOTIFY progressChanged)
    Q_PROPERTY(bool ADFMode READ ADFMode WRITE setADFMode NOTIFY ADFModeChanged)
    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(int colorMode READ colorMode WRITE setColorMode NOTIFY colorModeChanged)
    Q_PROPERTY(int scanAreaSize READ scanAreaSize WRITE setScanAreaSize NOTIFY scanAreaSizeChanged)
    Q_PROPERTY(bool MultiFeed READ MultiFeed WRITE setMultiFeed NOTIFY MultiFeedChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(bool AutoCropAndDeskew READ AutoCropAndDeskew WRITE setAutoCropAndDeskew NOTIFY AutoCropAndDeskewChanged)

    StringModel* getDeviceList(){return &m_deviceList;}
//    QStringList getDeviceList(){return m_deviceList;}
    ImageModel* getImageModel();
    qreal getProgress();
    bool ADFMode();
    int dpi();
    int colorMode();
    int scanAreaSize();
    bool MultiFeed();
    int contrast();
    int brightness();
    bool AutoCropAndDeskew();

    void setADFMode(bool value);
    void setDpi(int value);
    void setColorMode(int value);
    void setScanAreaSize(int value);
    void setMultiFeed(bool value);
    void setContrast(int value);
    void setBrightness(int value);
    void setAutoCropAndDeskew(bool value);

    Q_INVOKABLE void loadScannerSettings();
    Q_INVOKABLE void setDefault();
    Q_INVOKABLE void setOK();
    Q_INVOKABLE void log(const QString&);

    Q_INVOKABLE void test();

public:
    Scanner::Setting parseUiScannerSetting();
    Setter::struct_wifiSetting parseUiWifiSetting();
    QString parseUiPassword();
    int parseUiSaveTime();
    void uiParseSaveTime(int);
    void uiParseWifiInfo(Setter::struct_wifiInfo);
    void uiParseIpv4(Setter::struct_ipv4);
    Setter::struct_ipv4 parseUiIpv4();
    void uiParseSoftap(Setter::struct_softAp);
    Setter::struct_softAp parseUiSoftap();

signals:
    void progressChanged();
    void ADFModeChanged();
    void dpiChanged();
    void colorModeChanged();
    void scanAreaSizeChanged();
    void MultiFeedChanged();
    void contrastChanged();
    void brightnessChanged();
    void AutoCropAndDeskewChanged();

    void deviceCmd(int cmd);
    void cmdResult(int cmd,int result);
    void searchDeviceList();
    void cancelSearch();
    void searchComplete();

    void connectDevice(int);
public slots:
    void setProgress(qreal prgrs);
    void cancelScan();
    void addImage(QString ,QSize);
    void addDeviceInfo(QString);

private:
    qreal m_progress;
    DeviceManager* deviceManager;
    QMutex mutex;
    QThread thread;
    StringModel m_deviceList;
//    QStringList m_deviceList;
    ImageModel model;
    UI_Settings uiSettings;
    UI_Scanner_Settings scannerSettings;
};

#endif // JKINTERFACE_H
