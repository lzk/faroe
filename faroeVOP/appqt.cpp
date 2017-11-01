#include "appqt.h"
#include <QImage>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include "../lld/ImgFile/ImgFile.h"
#include "../thumbnailviewer/imagemodel.h"
AppQt::AppQt(QObject *parent) : QObject(parent)
  ,tmppath(QDir::tempPath() + "/scanimage")
{
    QDir dir = QDir(tmppath);
    if(!dir.exists()){
        if(dir.mkdir(tmppath))
            qDebug()<<"mkdir success";
    }
    qDebug()<<"tmp image path:" <<tmppath;

}

void AppQt::saveImage(char* filename ,unsigned char* buffer
                      ,int width ,int height ,int bitPerPixel)
{
    QImage::Format format = QImage::Format_RGB888;
    switch (bitPerPixel) {
    case IMG_1_BIT:        format = QImage::Format_Mono;/*Format_MonoLSB*/        break;
    case IMG_8_BIT:        format = QImage::Format_Grayscale8;        break;
    case IMG_24_BIT:        format = QImage::Format_RGB888;        break;
    default:
        break;
    }
    QImage image(buffer ,width ,height ,format);
    QString tmpFile = tmppath  + "/scaniamge_" + QDateTime::currentDateTime().toString("yyMMddHHmmsszzz") + QString(filename);
    QString thumFile;
    thumFile = ImageModel::getThumbnailFilename(tmpFile);

    QSize sourceSize;
    image.save(tmpFile);
    sourceSize = image.size();
    image.scaled(100 ,100).save(thumFile);
    emit addImage(tmpFile ,sourceSize);
}

void AppQt::updateProgress(float progress)
{
    emit progressChanged(progress);
}
