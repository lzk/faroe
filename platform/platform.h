#ifndef PLATFORM_H
#define PLATFORM_H

class QString;
class QStringList;
void platform_init();
const QString& getTempPath();

void saveMultiPageTiffImage(const QString& tmpPath ,const QString& imageFilePath);
void saveMultiPagePdfImageInit(const QString& filename);
bool saveMultiPagePdfImage(const QString& tmpPath ,bool firstPage);
void saveMultiPagePdfImageRelease();

void openEmail(const QString& subject ,const QString& recipient ,const QStringList& attachment);
void openApplication(const QString& appName ,const QStringList& fileList);

bool iCloudCheckLogin();
bool iCloudUpload(const QString& fileName);
#endif // PLATFORM_H
