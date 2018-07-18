#include "decodemanager.h"
#include <QImage>
#include <QFileInfo>
#include <QDebug>
extern QList<DMResult> zxing_decode(const QImage& image ,int decodeMode ,bool decodeMulti);
DecodeManager::DecodeManager(QObject *parent) : QObject(parent)
{
}

void DecodeManager::cancel(bool iscancel)
{
    m_cancel = iscancel;
}

struct DMDecodeResult DecodeManager::decode(const QString& fileName ,int decodeMode ,bool decodeMulti ,int angle ,int deep)
{
    struct DMDecodeResult dr;
    QImage image(fileName);
    if(image.isNull())
        return dr;
    dr.fileName = fileName;
    dr.width = image.width();
    dr.height = image.height();

    QFileInfo fileInfo(fileName);
    QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();

    QImage decodeImage;
    QList<DMResult> results;
    int index = 0;
    QRect dRect;
    QRectF rect;
    float fx,fy;
    for(int i = 0 ;i < angle ;i++){
        for(int j = 0 ;j < deep ;j++){
            if(m_cancel)
                break;
            switch (j) {
            case 0:
                decodeImage = image.scaled(
                            999,
                            999,
                            Qt::KeepAspectRatio,
                            Qt::FastTransformation);
                break;
            case 1:
                decodeImage = image;
                break;
            case 2:
            default:
                decodeImage = image.scaled(
                            2000,
                            2000,
                            Qt::KeepAspectRatio,
                            Qt::FastTransformation);
                break;
            }
            fx = image.width() * 1.0 / decodeImage.width();
            fy = image.height() * 1.0 / decodeImage.height();
            qDebug()<<"decode angle:"<<i*90 <<"deep:"<<j;
            results = zxing_decode(decodeImage ,decodeMode ,decodeMulti);
            for(auto result :results){
                if(!decodeMulti){
                    dr.result << result;
                    return dr;
                }
                bool exist = false;
                for(auto result_already :dr.result){
                    if(result_already.text == result.text){
                        exist = true;
                    }
                }
                if(exist)
                    continue;
                qDebug()<<"decode text:"<<result.text;
                qDebug()<<"decode type:"<<result.type;
                rect = result.rect;
                dRect = QRect(rect.x() * fx, rect.y() * fy, rect.width() * fx, rect.height() * fy);
                if(!dRect.isEmpty()){
                    result.width = dRect.width();
                    result.height = dRect.height();
                    result.fileName = preFileName + QString().sprintf("_%d.jpg" ,index++);
                    image.copy(dRect).transformed(QTransform().rotate(-90*i)).save(result.fileName ,"jpg");
                    qDebug()<<"decode filename:"<<result.fileName;
                    qDebug()<<"file size:"<<result.width<<"x"<<result.height;
                }
                dr.result << result;
            }
        }
        if(i < 3){
            image = image.transformed(QTransform().rotate(90));
        }
    }
    return dr;
}

struct DMDecodeResult DecodeManager::decodeMultiQrcode(const QString& fileName)
{
    return decode(fileName ,DECODETYPE_qrcode ,true);
}

struct DMDecodeResult DecodeManager::decodeMultiOneD(const QString& fileName)
{
    return decode(fileName ,DECODETYPE_barcode ,true);
}

QString DecodeManager::decodeOneD(const QString& fileName)
{
    struct DMDecodeResult dr = decode(fileName ,DECODETYPE_barcode ,false);
    if(dr.result.count() > 0)
        return dr.result.at(0).text;
    else
        return QString();
}

struct DMDecodeResult DecodeManager::decodeMultiAll(const QString& fileName)
{
    return decode(fileName ,DECODETYPE_allType ,true ,4);
}
