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

    void updateProgress(float progress);
    const char* getTempFilename(int side);
    bool saveScanImage(Scanner::Setting* setting ,int side ,int page ,int lines);
signals:
    void addImage(QString filename ,QSize sourceSize);
    void progressChanged(qreal progress);
public slots:

public:
    const QString tmppath;

private:
    int saveJpgFile(const char* tmpfilenmae ,const QString& filename ,Scanner::Setting* setting ,int lines);
};

#endif // APPQT_H
