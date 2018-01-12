#include "imagemodel.h"
#include <QDebug>
#include <QImage>
#include <QDir>
ImageItem::ImageItem(const QString &url ,QSize sourceSize ,int sn)
    :m_url(url) ,m_sourceSize(sourceSize) ,m_sn(sn)
{

}

QString ImageItem::url() const
{
    return m_url;
}
void ImageItem::setUrl(const QString& url)
{
    m_url = url;
}
int ImageItem::sn() const
{
    return m_sn;
}
void ImageItem::setSn(int sn)
{
    m_sn = sn;
}

QSize ImageItem::sourceSize() const
{
    return m_sourceSize;
}
void ImageItem::setSourceSize(QSize sourceSize)
{
    m_sourceSize = sourceSize;
}

ImageModel::ImageModel(QObject* parent)
    :QAbstractListModel(parent)
{

}

int ImageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.count();

}

QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if(index.row()<0 || index.row() >= m_list.count())
        return QVariant();
    const ImageItem &ip = m_list[index.row()];
    if(role == UrlRole)
        return ip.url();
    else if(role == SnRole)
        return ip.sn();
    else if(role == SourceSizeRole)
        return ip.sourceSize();
    return QVariant();
}

QHash<int,QByteArray> ImageModel::roleNames() const
{
    QHash<int ,QByteArray> roles;
    roles[UrlRole] = "url";
    roles[SnRole] = "sn";
    roles[SourceSizeRole] = "sourceSize";
    return roles;
}

bool ImageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool isDataChanged = false;
    if (index.isValid()) {
        ImageItem &ip = m_list[index.row()];
        if(role == UrlRole){
            ip.setUrl(value.toString());
            isDataChanged = true;
        }else if(role == SnRole){
            ip.setSn(value.toInt());
            isDataChanged = true;
        }else if(role == SourceSizeRole){
            ip.setSourceSize(value.toSize());
            isDataChanged = true;
        }
        if(isDataChanged){
            //emit dataChanged(index, index);//list out of range?
            emit dataChanged(index, index ,QVector<int>(1 ,role));
        }
    }
    return isDataChanged;
}

QVariant ImageModel::get(int row)
{
//    qDebug()<<__func__;
    if(row >= 0)
    {
//        const ImageItem* item = &m_list.at(row);
//        QMap<QString, QVariant> itemData;
//        itemData.insert("url" ,QVariant(item->url()));
//        itemData.insert("sn" ,QVariant(item->sn()));
//        return QVariant(itemData);
        return QVariant(&m_list.at(row));
    }
    return QVariant();
}

int ImageModel::getCount() const
{
    return m_list.count();
}

void ImageModel::saveRotatoedImage(int row ,int angle)
{
    if(row<0 || row >= m_list.count())
        return ;
    int ag = angle % 4 * 90;
    if(ag != 0){
        QString filename = m_list.at(row).url();
        QImage image = QImage(filename);
//        QString newFilename = filename;
        QString newFilename = getTempFilename(filename ,QString::asprintf("%d_" ,ag));
        image.transformed(QTransform().rotate(ag)).save(newFilename);

        QImage(getThumbnailFilename(filename)).transformed(QTransform().rotate(ag)).save(getThumbnailFilename(newFilename));
        setData(index(row) ,QVariant(newFilename) ,UrlRole);
        QFile::remove(filename);
        QFile::remove(getThumbnailFilename(filename));
    }
}

void ImageModel::addImage(const ImageItem& ip)
{
    beginInsertRows(QModelIndex() ,rowCount() ,rowCount());
    m_list << ip;
    endInsertRows();
}

void ImageModel::remove(int row ,int count)
{
    QString filename = m_list.at(row).url();
    QFile::remove(filename);
    QFile::remove(getThumbnailFilename(filename));
    beginRemoveRows(QModelIndex() ,row ,row + count -1);
    m_list.removeAt(row);
    endRemoveRows();
}

void ImageModel::removeAll()
{
    int count = m_list.count();
    if(count < 1)
        return;
    QString filename;
    beginRemoveRows(QModelIndex(), 0, count - 1);
    for (int row = 0; row < count; ++row) {
        filename = m_list.last().url();
        if(QFile::exists(filename)){
            QFile::remove(filename);
            QFile::remove(getThumbnailFilename(filename));
        }
        m_list.removeLast();
    }
    endRemoveRows();
}

QString ImageModel::getTempFilename(const QString& origin_file_name ,const QString& str)
{
    QFileInfo info(origin_file_name);
    return info.dir().absolutePath() +"/" + str + info.fileName();
}

QString ImageModel::getThumbnailFilename(const QString& origin_file_name)
{
    return getTempFilename(origin_file_name ,"thmb_");
}

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
void ImageModel::toPrint(QList<int> selectedList)
{
    if(selectedList.length()<=0){
        return;
    }
    QPrintDialog printDialog;
    if (printDialog.exec() == QDialog::Accepted) {
        // print ...
        QPrinter* printer = printDialog.printer();
        qDebug()<<"printer:"<<printer->printerName();
        QPainter painter(printer);
        QRect rect = painter.viewport();
        for(int i = 0 ;i < selectedList.length() ;i++) {
            QPixmap pixmap(m_list.at(selectedList[i]).url());
            QSize size = pixmap.size();
            if(!size.isValid())
                continue;
            if(i)
                printer->newPage();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(pixmap.rect());
            painter.drawPixmap(0,0,pixmap);
        }
    }
}

#include "../jkenums.h"
void ImageModel::toEmail(QList<int> selectedList ,int fileType)
{
    if(selectedList.length()<=0){
        return;
    }
    qDebug()<<"file type:" << fileType;
}

void saveFile(const QString& target ,const QString& source ,const QString& suffix){

}
#include <QUrl>
#include <QProcess>
void ImageModel::toFile(QList<int> selectedList ,const QString& filePath)
{
    if(selectedList.length()<=0){
        return;
    }
    QString fileName = QUrl(filePath).toLocalFile();
    QString fullFileName;
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();
    qDebug()<<"pre file name:" <<fileName;
    if(!suffix.compare("jpg")){
        for (int i = 0 ;i < selectedList.length() ;i++){
            if(selectedList.length() == 1)
                fullFileName = fileName;
            else
                fullFileName = preFileName + QString().sprintf("_%d." ,i) +suffix;
            qDebug()<<"save " << m_list.at(selectedList[i]).url();
            qDebug()<<"to " << fullFileName;
            QFile::copy(m_list.at(selectedList[i]).url() ,fullFileName);
        }
    }else if(!suffix.compare("bmp")){
        for (int i = 0 ;i < selectedList.length() ;i++){
            if(selectedList.length() == 1)
                fullFileName = fileName;
            else
                fullFileName = preFileName + QString().sprintf("_%d." ,i) +suffix;
            QImage image(m_list.at(selectedList[i]).url());
            image.save(fullFileName ,"bmp");
        }
    }else if(!suffix.compare("pdf")){
        QPrinter print;
        QPrinter* printer = &print;
        qDebug()<<"printer:"<<printer->printerName();
//        printer->setPageSize(QPrinter::A4); //设置纸张大小为A4
        printer->setOutputFormat(QPrinter::PdfFormat); //设置输出格式为pdf
        printer->setOutputFileName(fileName);  //设置输出路径
        QPainter painter(printer);
        QRect rect = painter.viewport();
        for(int i = 0 ;i < selectedList.length() ;i++) {
            QPixmap pixmap(m_list.at(selectedList[i]).url());
            QSize size = pixmap.size();
            if(!size.isValid())
                continue;
            if(i)
                printer->newPage();
//            size.scale(rect.size(), Qt::KeepAspectRatio);
            printer->setPageSizeMM(size);
//            rect = painter.viewport();
//            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//            painter.setWindow(pixmap.rect());
            painter.drawPixmap(0,0,pixmap);
        }

    }else if(!suffix.compare("tif")){
        QString tmpFile = preFileName + "tmp.tif";
        QFile::remove(fullFileName);
        for (int i = 0 ;i < selectedList.length() ;i++){
            QImage image(m_list.at(selectedList[i]).url());
            image.save(tmpFile ,"tiff");
//            if(i){
//                QProcess().start("tiffutil" ,QStringList()<<"-cat"<<fullFileName<<tmpFile<<"-out"<<fullFileName);
//            }else{
//                QProcess().start("tiffutil" ,QStringList()<<"-cat"<<tmpFile<<"-out"<<fullFileName);
//            }
        }

    }
}

void ImageModel::toFTP(QList<int> selectedList ,QJSValue obj)
{
    QString serverAddress = obj.property("serverAddress").toString();
    QString userName = obj.property("userName").toString();
    QString password = obj.property("password").toString();
    QString targetPath = obj.property("targetPath").toString();
//    qDebug()<<"serverAddress:" << serverAddress;
//    qDebug()<<"userName:" << userName;
//    qDebug()<<"password:" << password;
//    qDebug()<<"targetPath:" << targetPath;
    QUrl url;
    url.setScheme("ftp");
    url.setHost(serverAddress);
    // 这里是设置远端主机上传时的文件名，不设置则名称为本地端上传名称
    url.setUserName(userName);
    url.setPassword(password);
    url.setPort(21);
    foreach (int i, selectedList) {
        url.setPath(targetPath + "/" + QFileInfo(m_list.at(selectedList[i]).url()).fileName());
        qDebug()<<"FTP url:" <<url;
//        QFile file(m_list.at(selectedList[i]).url());
//        if(file.open(QIODevice::ReadOnly)){

//        }
    }
}

void ImageModel::toApplication(QList<int> selectedList ,const QString& fileName)
{
    qDebug()<<"file name:" << fileName;
}

void ImageModel::toCloud(QList<int> selectedList ,QJSValue obj)
{
    int cloudType = obj.property("cloudType").toInt();
    qDebug()<<"cloudType:"<<cloudType;
}
