#include <QString>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>
#include "../log.h"

//extern void saveMultiPageTiffImage(NSString* tmpPath ,NSString* imageFilePath);
//void saveMultiPageTiffImage(const QString& tmpPath ,const QString& imageFilePath)
//{
//    return saveMultiPageTiffImage(tmpPath.toNSString() ,imageFilePath.toNSString());
//}

static QPrinter* printer = NULL;
static QPainter* painter = NULL;
void saveMultiPagePdfImageInit(const QString& fileName)
{
    printer = new QPrinter;
    printer->setPageSize(QPrinter::A4); //设置纸张大小为A4
    printer->setOutputFormat(QPrinter::PdfFormat); //设置输出格式为pdf
    printer->setOutputFileName(fileName);  //设置输出路径
    painter = new QPainter(printer);
}

bool saveMultiPagePdfImage(const QString& tmpPath ,bool firstPage)
{
    QRect rect = painter->viewport();
    if(rect.isEmpty())
        return false;
    QPixmap pixmap = QPixmap(tmpPath).scaled(rect.size() ,Qt::KeepAspectRatio);
    QSize size = pixmap.size();
    if(!size.isValid())
        return false;
    if(!firstPage)
        printer->newPage();
    painter->drawPixmap(rect.x() + (rect.width() - size.width()) / 2,
                       rect.y() + (rect.height() - size.height()) / 2,pixmap);
    return true;
}

void saveMultiPagePdfImageRelease()
{
    if(painter)
        delete painter;
    painter = NULL;
    if(printer)
        delete printer;
    printer = NULL;
}
