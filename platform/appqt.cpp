#include "appqt.h"
#include <QImage>
#include <QDir>
#include <QDateTime>
#include "log.h"
#include "platform.h"
#include "../lld/device.h"
AppQt::AppQt(QObject *parent) : QObject(parent)
{

}

void AppQt::updateProgress(int progress ,int page)
{
    emit progressChanged(progress ,page);
}

const char* AppQt::getTempFilename(int side)
{
    QString tmppath = getTempPath();
    return (tmppath + "/tmpimage"  + (side ?"B":"A")).toLatin1().data();
}

bool AppQt::saveScanImage(Scanner::Setting* setting ,Scanner::Para_Extra* para)
{
    int err = -1;
    QString tmppath = getTempPath();
//    char colorType = 'C';
//    switch (setting->BitsPerPixel) {
//    case SETTING_IMG_1_BIT:     colorType = 'B';        break;
//    case SETTING_IMG_8_BIT:     colorType = 'G';        break;
//    case SETTING_IMG_24_BIT:    colorType = 'C';        break;
//    default:        break;
//    }
//    QString saveFileName = tmppath  + "/scaniamge_" + QDateTime::currentDateTime().toString("yyMMddHHmmsszzz")
//            + QString().sprintf("_%c%d*%d_%c%02d" ,colorType ,setting->dpi_x ,setting->dpi_y ,side?'B':'A' ,page);
    QString saveFileName = tmppath  + "/img" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    if(setting->ADFMode == SETTING_SCAN_AB_SIDE){
        saveFileName += QString().sprintf("%03d%c" ,para->page[para->dup] ,para->dup ?'B' :'A');
    }else{
        saveFileName += QString().sprintf("%03d" ,para->page[para->dup]);
    }
    if(setting->bColorDetect){
        if(para->isColor){
            saveFileName += "_color";
        }else{
            saveFileName += "_gray";
        }
    }
    const char* scanFileName = getTempFilename(para->dup);
    err = saveJpgFile(scanFileName ,para);
    if(err)
        return false;
    switch (setting->format) {
    case SETTING_IMG_FORMAT_JPG:
        saveFileName += ".JPG";
        QFile::rename(scanFileName ,saveFileName);
        break;
    default:
        break;
    }
    if(setting->bSkipBlankPage && para->isBlank){
        QFile::remove(saveFileName);
        return true;
    }
    if(setBrightnessAndContrast(saveFileName ,setting->brightness ,setting->contrast))
        return false;
    return true;
}

int AppQt::saveJpgFile(const char* filename ,Scanner::Para_Extra* para)
{
    FILE* file = fopen(filename ,"rb+");
    if(!file)
        return -1;
    if (para->lines >= 8) {
        unsigned char dpi[5] = { 0x01, 0x01, 0x2c, 0x01, 0x2c };
        unsigned char height[2];
        dpi[0] = 0x01;
        dpi[1] = (unsigned char)(para->dpi_x >> 8);
        dpi[2] = (unsigned char)(para->dpi_x);
        dpi[3] = (unsigned char)(para->dpi_y >> 8);
        dpi[4] = (unsigned char)para->dpi_y;
        para->lines -= 7;
        height[0] = (unsigned char)(para->lines >> 8);
        height[1] = (unsigned char)para->lines;
        fseek(file, 0x0d, SEEK_SET);
        fwrite(dpi, 1, sizeof(dpi), file);
        fseek(file, (para->isColor ? 0xe6 : 0x64), SEEK_SET);
        fwrite(height, 1, sizeof(height), file);
        fflush(file);
    }
    fclose(file);
    return 0;
}

#include <QQmlEngine>
#include <QQmlComponent>
int AppQt::setBrightnessAndContrast(const QString& fileName ,int brightness ,int contrast)
{
#if 1
    completed(fileName);
    return 0;
#else
    if(brightness == 50 && contrast == 50){
        completed(fileName);
        return 0;
    }

    QQmlEngine engine;
    QQmlComponent component(&engine, "qrc:/newui/component/BrightnessAndContrast.qml" ,QQmlComponent::PreferSynchronous);
    QObject *rootObject = component.create();

    bool ret = false;
    if(rootObject){
        QObject::connect(rootObject ,SIGNAL(completed(QString)) ,this ,SLOT(completed(const QString&)));
        ret = QMetaObject::invokeMethod(rootObject ,"save"
                                        ,Q_ARG(QVariant ,fileName)
                                        ,Q_ARG(QVariant ,brightness)
                                        ,Q_ARG(QVariant ,contrast)
                                        );
    }
    return ret ?0 :-1;
#endif
}

#include <QQuickItem>
void AppQt::completed(const QString& fileName)
{
    qDebug()<<"save complete";
    QObject* object = sender();
    if(qobject_cast<QQuickItem*>(object)){
        delete object;
    }
    emit addImage(fileName);
}
