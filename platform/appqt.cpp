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

QString AppQt::getStillTempFilename(int side)
{
    return  getTempPath() + "/tmpimage"  + (side ?"B":"A");
}

const char* AppQt::getTempFilename(int side)
{
    QString tmppath = getStillTempFilename(side);
    return tmppath.toLatin1().constData();//
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
    QString scanFileName = getStillTempFilename(para->dup);
    err = saveJpgFile(scanFileName.toLatin1().constData() ,para);
    if(err){
        LOG_NOPARA("save jpg fail");
        return false;
    }
    switch (setting->format) {
    case SETTING_IMG_FORMAT_JPG:
        saveFileName += ".JPG";
        if(!QFile::rename(scanFileName ,saveFileName)){
            LOG_NOPARA("rename fail " + scanFileName + " to " +saveFileName);
        }
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
    emit addImage(saveFileName);
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

int AppQt::setBrightnessAndContrast(const QString& fileName ,int brightness ,int contrast)
{
    if(brightness == 50 && contrast == 50){
        return 0;
    }

    QImage image(fileName);
    if(image.isNull())
        return -1;
    uchar* line = image.scanLine(0);
    uchar* pixel = line;

    float bb = (brightness - 50) * 0.01;
    float cc = (contrast) * 0.02;
    bb = qBound(-0.3f ,bb ,0.3f);
    cc = cc < 0.5 ?0.5 :cc;
    bb += 0.5 * (1 - cc);

    uchar tabel[256];
    for(int i = 0 ;i < 256 ;i++){
        tabel[i] = qBound(0 ,(int)(cc * i + bb * 255) ,255);
    }
    int height = image.height();
    int width = image.width();
    int bytesPerLine = image.bytesPerLine();
    qDebug()<<"image format:"<<image.format();
    switch (image.format()) {
    case QImage::Format_ARGB32:
    case QImage::QImage::Format_RGB32:
        for(int y = 0 ;y < height ;y++){
            pixel = line;
            for(int x = 0 ;x < width ;x++){
                pixel[0] = tabel[pixel[0]];
                pixel[1] = tabel[pixel[1]];
                pixel[2] = tabel[pixel[2]];
                pixel += 4;
            }
            line += bytesPerLine;
        }
        break;
    case QImage::Format_Grayscale8:
        for(int y = 0 ;y < height ;y++){
            pixel = line;
            for(int x = 0 ;x < width ;x++){
                pixel[0] = tabel[pixel[0]];
                pixel ++;
            }
            line += bytesPerLine;
        }
        break;
    case QImage::QImage::Format_RGB888:
        for(int y = 0 ;y < height ;y++){
            pixel = line;
            for(int x = 0 ;x < width ;x++){
                pixel[0] = tabel[pixel[0]];
                pixel[1] = tabel[pixel[1]];
                pixel[2] = tabel[pixel[2]];
                pixel += 3;
            }
            line += bytesPerLine;
        }
        break;
    default:
        break;
    }
    image.save(fileName);
    return 0;
}

