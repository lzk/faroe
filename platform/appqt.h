#ifndef APPQT_H
#define APPQT_H

#include <QObject>
#include <QSize>
#include "../lld/platformapp.h"
using namespace JK;
class AppQt : public QObject ,PlatformApp
{
    Q_OBJECT
public:
    explicit AppQt(QObject *parent = nullptr);

    void updateProgress(int progress ,int page);
    const char* getTempFilename(int side);
    bool saveScanImage(Scanner::Setting* setting ,Scanner::Para_Extra* para);
signals:
    void addImage(QString filename);
    void progressChanged(int progress ,int page);

private:
    int saveJpgFile(const char* tmpfilenmae ,Scanner::Para_Extra* para);
    int setBrightnessAndContrast(const QString& fileName ,int brightness ,int contrast);
};

#endif // APPQT_H
