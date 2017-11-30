#include "appqt.h"
#include <QImage>
#include <QDir>
#include <QDateTime>
#include "log.h"
#include "../lld/device.h"
#include "../thumbnailviewer/imagemodel.h"
AppQt::AppQt(QObject *parent) : QObject(parent)
  ,tmppath(QDir::tempPath() + "/scanimage")
{
    QDir dir = QDir(tmppath);
    if(!dir.exists()){
        if(dir.mkdir(tmppath))
            LOG_NOPARA("mkdir success");
    }
    LOG_NOPARA("tmp image path:" + tmppath);

}

void AppQt::updateProgress(float progress)
{
    emit progressChanged(progress);
}

const char* AppQt::getTempFilename(int side)
{
    return (tmppath + "/tmpimage"  + (side ?"B":"A")).toLatin1().data();
}

bool AppQt::saveScanImage(Scanner::Setting* setting ,int side ,int page ,int lines)
{
    int err = -1;
    char colorType = 'C';
    switch (setting->BitsPerPixel) {
    case SETTING_IMG_1_BIT:     colorType = 'B';        break;
    case SETTING_IMG_8_BIT:     colorType = 'G';        break;
    case SETTING_IMG_24_BIT:    colorType = 'C';        break;
    default:        break;
    }
    QString scanFilename = tmppath  + "/scaniamge_" + QDateTime::currentDateTime().toString("yyMMddHHmmsszzz")
            + QString().sprintf("_%c%d*%d_%c%02d" ,colorType ,setting->dpi_x ,setting->dpi_y ,side?'B':'A' ,page);
    switch (setting->format) {
    case SETTING_IMG_FORMAT_JPG:
        scanFilename += ".jpg";
        err = saveJpgFile(getTempFilename(side) ,scanFilename ,setting ,lines);
        break;
    default:
        break;
    }
    if(err)
        return false;

    QString thumFilename = ImageModel::getThumbnailFilename(scanFilename);
    QSize sourceSize;
    QImage image;
    if(image.load(scanFilename)){
        sourceSize = image.size();
        image.scaled(100 ,100).save(thumFilename);
        emit addImage(scanFilename ,sourceSize);
        return true;
    }
    return false;
}

int AppQt::saveJpgFile(const char* tmpfilenmae ,const QString& filename ,Scanner::Setting* setting ,int lines)
{
    FILE* file = fopen(tmpfilenmae ,"rb+");
    if(!file)
        return -1;
    if (lines >= 8) {
        unsigned char dpi[5] = { 0x01, 0x01, 0x2c, 0x01, 0x2c };
        unsigned char height[2];
        dpi[0] = 0x01;
        dpi[1] = (unsigned char)(setting->dpi_x >> 8);
        dpi[2] = (unsigned char)(setting->dpi_x);
        dpi[3] = (unsigned char)(setting->dpi_y >> 8);
        dpi[4] = (unsigned char)setting->dpi_y;
        lines -= 7;
        height[0] = (unsigned char)(lines >> 8);
        height[1] = (unsigned char)lines;
        fseek(file, 0x0d, SEEK_SET);
        fwrite(dpi, 1, sizeof(dpi), file);
        fseek(file, ((setting->BitsPerPixel >= 24) ? 0xe6 : 0x64), SEEK_SET);
        fwrite(height, 1, sizeof(height), file);
        fflush(file);
    }
    fclose(file);
    QFile::rename(tmpfilenmae ,filename);
    return 0;
}
