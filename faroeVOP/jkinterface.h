#ifndef JKINTERFACE_H
#define JKINTERFACE_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QImage>
#include "../thumbnailviewer/imagemodel.h"
#include "devicemanager.h"
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

    void scan();
    void deviceChanged(const QString&);
    void scanResult(int result);

public slots:
    void setProgress(qreal prgrs);
    void cancelScan();
    void addImage(QString ,QSize);

private:
    qreal m_progress;
    DeviceManager* deviceManager;
    QMutex mutex;
    QThread thread;
    ImageModel model;
    UI_Settings uiSettings;
    UI_Scanner_Settings scannerSettings;
};

#endif // JKINTERFACE_H
