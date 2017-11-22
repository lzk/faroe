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

    void saveImage(char* ,unsigned char* buffer
                   ,int width ,int height ,int bitPerPixel);
    void updateProgress(float progress);
signals:
    void addImage(QString filename ,QSize sourceSize);
    void progressChanged(qreal progress);
public slots:

public:
    const QString tmppath;
};

#endif // APPQT_H
