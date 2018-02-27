#include "decodemanager.h"
#include <QDebug>
#include <QFileInfo>
#include <zxing/multi/GenericMultipleBarcodeReader.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/DecodeHints.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
using std::vector;
#include <zxing/oned/MultiFormatOneDReader.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/common/detector/WhiteRectangleDetector.h>
#include "WrapperQImage.h"
DecodeManager::DecodeManager(QObject *parent) : QObject(parent)
{
}

struct DMDecodeResult DecodeManager::decodeMultiQrcode(const QString& fileName)
{
    DecodeHints hints = DecodeHints::QR_CODE_HINT;
    hints.setTryHarder(true);
//    zxing::qrcode::QRCodeReader reader;
    zxing::MultiFormatReader reader;
    return decodeMulti(fileName ,&reader ,hints);
}

struct DMDecodeResult DecodeManager::decodeMultiOneD(const QString& fileName)
{
    DecodeHints hints = DecodeHints::ONED_HINT;
    hints.setTryHarder(true);
//    zxing::oned::MultiFormatOneDReader reader(hints);
    zxing::MultiFormatReader reader;
    return decodeMulti(fileName ,&reader ,hints);
}

QString DecodeManager::decodeOneD(const QString& fileName)
{
    DecodeHints hints = DecodeHints::ONED_HINT;
    hints.setTryHarder(true);
//    zxing::oned::MultiFormatOneDReader reader(hints);
    zxing::MultiFormatReader reader;
    struct DMDecodeResult dr = decode(fileName ,&reader ,hints);
    if(dr.result.count() > 0)
        return dr.result.at(0).text;
    else
        return QString();
}

struct DMDecodeResult DecodeManager::decodeMultiAll(const QString& fileName)
{
    DecodeHints hints = DecodeHints::DEFAULT_HINT
    | DecodeHints::MAXICODE_HINT
    | DecodeHints::RSS_EXPANDED_HINT
    | DecodeHints::UPC_EAN_EXTENSION_HINT
    | DecodeHints::ASSUME_GS1;
    hints.setTryHarder(true);
    zxing::MultiFormatReader reader;
    return decodeMulti(fileName ,&reader ,hints);
}


static QRectF getTagRect(const ArrayRef<Ref<ResultPoint> > &resultPoints, const Ref<BitMatrix> &bitMatrix);

QRectF getTagRect(const ArrayRef<Ref<ResultPoint> > &resultPoints, const Ref<BitMatrix> &bitMatrix)
{
    if (resultPoints->size() < 2)
        return QRectF();

    int matrixWidth = bitMatrix->getWidth();
    int matrixHeight = bitMatrix->getHeight();
    // 1D barcode
    if (resultPoints->size() == 2) {
        WhiteRectangleDetector detector(bitMatrix);
        std::vector<Ref<ResultPoint> > resultRectPoints = detector.detect();

        if (resultRectPoints.size() != 4)
            return QRectF();

        qreal xMin = resultPoints[0]->getX();
        qreal xMax = xMin;
        for (unsigned int i = 1; i < resultPoints->size(); ++i) {
            qreal x = resultPoints[i]->getX();
            if (x < xMin)
                xMin = x;
            if (x > xMax)
                xMax = x;
        }

        qreal yMin = resultRectPoints[0]->getY();
        qreal yMax = yMin;
        for (unsigned int i = 1; i < resultRectPoints.size(); ++i) {
            qreal y = resultRectPoints[i]->getY();
            if (y < yMin)
                yMin = y;
            if (y > yMax)
                yMax = y;
        }

        return QRectF(QPointF(xMin / matrixWidth, yMin / matrixHeight), QPointF(xMax / matrixWidth, yMax / matrixHeight));
    }

    // 2D QR code
    if (resultPoints->size() == 4) {
        qreal xMin = resultPoints[0]->getX();
        qreal xMax = xMin;
        qreal yMin = resultPoints[0]->getY();
        qreal yMax = yMin;
        for (unsigned int i = 1; i < resultPoints->size(); ++i) {
            qreal x = resultPoints[i]->getX();
            qreal y = resultPoints[i]->getY();
            if (x < xMin)
                xMin = x;
            if (x > xMax)
                xMax = x;
            if (y < yMin)
                yMin = y;
            if (y > yMax)
                yMax = y;
        }

        return QRectF(QPointF(xMin / matrixWidth, yMin / matrixHeight), QPointF(xMax / matrixWidth, yMax / matrixHeight));
    }

    return QRectF();
}

struct DMDecodeResult DecodeManager::decodeMulti(const QString& fileName ,zxing::Reader* reader ,DecodeHints hints)
{
    struct DMDecodeResult dr;
    QImage image(fileName);
    if(image.isNull())
        return dr;

    zxing::multi::GenericMultipleBarcodeReader _decoder(*reader);
    zxing::multi::GenericMultipleBarcodeReader* decoder = &_decoder;

    vector<Ref<Result> > resList;// ,resList2;
    struct DMResult result;
    QString string;
    QString format;
    QString charSet;
    QTextCodec *codec;
    QRectF rect;

    WrapperQImage *ciw;
//    ciw = WrapperQImage::Factory(image);
    ciw = WrapperQImage::Factory(image, 999, 999, true);
    try{
        Ref<LuminanceSource> imageRef(ciw);
        Ref<GlobalHistogramBinarizer> binz( new GlobalHistogramBinarizer(imageRef) );
        Ref<BinaryBitmap> bb( new BinaryBitmap(binz) );

        bool hasSucceded = false;
        try {
            resList = decoder->decodeMultiple(bb, hints);
            hasSucceded = true;
        }catch(zxing::Exception &e){
            qDebug()<<"zxing error:"<<QString(e.what());
        }

//        if ( bb->isRotateSupported()) {
//            Ref<BinaryBitmap> bbTmp = bb;

//            for (int i=0; i<3; i++) {
//                Ref<BinaryBitmap> rotatedImage(bbTmp->rotateCounterClockwise());
//                bbTmp = rotatedImage;

//                resList2 = decoder->decodeMultiple(rotatedImage, hints);
//                resList.insert(resList.end() ,resList2.begin() ,resList2.end());//need rm same text
//            }
//        }

        QFileInfo fileInfo(fileName);
        QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();

        dr.fileName = fileName;
        dr.width = image.width();
        dr.height = image.height();
        int index = 0;
        int fmt;
        for (auto res:resList){
            string = QString(res->getText()->getText().c_str());
            if (!string.isEmpty() && (string.length() > 0)) {
                fmt = res->getBarcodeFormat().value;
                format = decoderFormatToString(1 << fmt);
//                charSet = QString::fromStdString(res->getCharSet());
//                if (!charSet.isEmpty()) {
//                    codec = QTextCodec::codecForName(res->getCharSet().c_str());
//                    if (codec)
//                        string = codec->toUnicode(res->getText()->getText().c_str());
//                }
                rect = getTagRect(res->getResultPoints(), binz->getBlackMatrix());
            }
            result.format = format;
            result.charSet = charSet;
            result.text = string;
            QRect dRect = QRect(QPoint(rect.topLeft().x() * dr.width ,rect.topLeft().y() * dr.height)
                           ,QPoint(rect.bottomRight().x() * dr.width ,rect.bottomRight().y() * dr.height));

            result.width = dRect.width();
            result.height = dRect.height();
            if(!dRect.isEmpty()){
                result.fileName = preFileName + QString().sprintf("_%d.jpg" ,index++);
                image.copy(dRect).save(result.fileName ,"jpg");
            }
            dr.result << result;
        }
    } catch(zxing::Exception &e) {
        qDebug()<<"zxing error:"<<QString(e.what());
        return dr;
    }
    return dr;
}

struct DMDecodeResult DecodeManager::decode(const QString& fileName ,zxing::Reader* decoder ,DecodeHints hints)
{
    struct DMDecodeResult dr;
    QImage image(fileName);
    if(image.isNull())
        return dr;

    bool tryHarder_ = true;
    Ref<Result> res;

    struct DMResult result;
    QString string;
    QString format;
//    QString charSet;
//    QTextCodec *codec;
    QRectF rect;

    WrapperQImage *ciw;
    ciw = WrapperQImage::Factory(image, 999, 999, true);
//    ciw = WrapperQImage::Factory(image);
    try{
        Ref<LuminanceSource> imageRef(ciw);
        Ref<GlobalHistogramBinarizer> binz( new GlobalHistogramBinarizer(imageRef) );
        Ref<BinaryBitmap> bb( new BinaryBitmap(binz) );

        bool hasSucceded = false;
        try {
            res = decoder->decode(bb, hints);
            hasSucceded = true;
        }catch(zxing::Exception &e){}
        if(!hasSucceded)
        {
            hints.setTryHarder(true);

            try {
                res = decoder->decode(bb, hints);
                hasSucceded = true;
            } catch(zxing::Exception &e) {}

            if (tryHarder_ && bb->isRotateSupported()) {
                Ref<BinaryBitmap> bbTmp = bb;

                for (int i=0; (i<3 && !hasSucceded); i++) {
                    Ref<BinaryBitmap> rotatedImage(bbTmp->rotateCounterClockwise());
                    bbTmp = rotatedImage;

                    try {
                        res = decoder->decode(rotatedImage, hints);
//                        processingTime = t.elapsed();
                        hasSucceded = true;
                    } catch(zxing::Exception &e) {}
                }
            }
        }

        QFileInfo fileInfo(fileName);
        QString preFileName = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();

        dr.fileName = fileName;
        dr.width = image.width();
        dr.height = image.height();
        int index = 0;
//            for (auto res:resList){
            int fmt;
            string = QString(res->getText()->getText().c_str());
            if (!string.isEmpty() && (string.length() > 0)) {
                fmt = res->getBarcodeFormat().value;
                format = decoderFormatToString(fmt);
//                charSet = QString::fromStdString(res->getCharSet());
//                if (!charSet.isEmpty()) {
//                    codec = QTextCodec::codecForName(res->getCharSet().c_str());
//                    if (codec)
//                        string = codec->toUnicode(res->getText()->getText().c_str());
//                }
                rect = getTagRect(res->getResultPoints(), binz->getBlackMatrix());
            }
            result.text = string;
            result.width = rect.width();
            result.height = rect.height();
            if(!rect.isEmpty()){
                result.fileName = preFileName + QString().sprintf("_%d.jpg" ,index++);
                image.copy(rect.toRect()).save(result.fileName ,"jpg");
            }
            dr.result << result;
//            }
    } catch(zxing::Exception &e) {
        qDebug()<<"zxing error:"<<QString(e.what());
        struct DMDecodeResult dd;
        return dd;
    }
    return dr;
}

QString DecodeManager::decoderFormatToString(int fmt)
{
    switch (fmt) {
    case DecoderFormat_Aztec:
        return "AZTEC";

    case DecoderFormat_CODABAR:
        return "CODABAR";

    case DecoderFormat_CODE_39:
        return "CODE_39";

    case DecoderFormat_CODE_93:
        return "CODE_93";

    case DecoderFormat_CODE_128:
        return "CODE_128";

    case DecoderFormat_CODE_128_GS1:
        return "CODE_128_GS1";

    case DecoderFormat_DATA_MATRIX:
        return "DATA_MATRIX";

    case DecoderFormat_EAN_8:
        return "EAN_8";

    case DecoderFormat_EAN_13:
        return "EAN_13";

    case DecoderFormat_ITF:
        return "ITF";

    case DecoderFormat_MAXICODE:
        return "MAXICODE";

    case DecoderFormat_PDF_417:
        return "PDF_417";

    case DecoderFormat_QR_CODE:
        return "QR_CODE";

    case DecoderFormat_RSS_14:
        return "RSS_14";

    case DecoderFormat_RSS_EXPANDED:
        return "RSS_EXPANDED";

    case DecoderFormat_UPC_A:
        return "UPC_A";

    case DecoderFormat_UPC_E:
        return "UPC_E";

    case DecoderFormat_UPC_EAN_EXTENSION:
        return "UPC_EAN_EXTENSION";
    } // switch
    return QString();
}
