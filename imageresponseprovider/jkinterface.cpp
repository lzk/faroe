#include "jkinterface.h"
#include <QDir>
#include <QDateTime>
#include <QImage>
#include <QDebug>
JKInterface::JKInterface(QObject *parent) : QObject(parent)
  ,tmppath(QDir::tempPath() + "/scanimage")
{
    QDir dir = QDir(tmppath);
    if(!dir.exists()){
        if(dir.mkdir(tmppath))
            qDebug()<<"mkdir success";
    }
    qDebug()<<"tmp image path:" <<tmppath;
}


ImageModel* JKInterface::getImageModel()
{
    return &model;
}

void JKInterface::add()
{
    static int i = 0;
    i++;
    if(i == 16)
        i = 1;
    QString sourceFile = QString::asprintf("E:/tmp/pic/%d.jpg" ,i);

    QString tmpFile = tmppath + "/tmpscaniamge_" + QDateTime::currentDateTime().toString("yyMMddHHmmsszzz") +".jpg";
    QString thumFile = ImageModel::getThumbnailFilename(tmpFile);
    QFile::copy(sourceFile ,tmpFile);
    QImage image =QImage(tmpFile);
    QSize sourceSize = image.size();
    if(!image.isNull()){
        image.scaled(100 ,100).save(thumFile);
//        model.addImage(ImageItem(tmpFile ,sourceSize));
    }
    model.addImage(ImageItem(tmpFile ,sourceSize));//add empty image for test
}

