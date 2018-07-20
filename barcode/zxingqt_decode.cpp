
#include <zxing/multi/GenericMultipleBarcodeReader.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/DecodeHints.h>
#include <zxing/common/HybridBinarizer.h>
using std::vector;
#include <zxing/oned/MultiFormatOneDReader.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/common/detector/WhiteRectangleDetector.h>
#include "WrapperQImage.h"

#include "decodemanager.h"
#include <QRegularExpression>
#include <QDebug>
static vector<Ref<Result> > qt_decode(const QImage& image ,zxing::Reader* reader ,bool decodeMulti)
{
    vector<Ref<Result> > results;
    if(image.isNull())
        return results;

    WrapperQImage *ciw = WrapperQImage::Factory(image);
    Ref<LuminanceSource> imageRef(ciw);
    Ref<HybridBinarizer> binz( new HybridBinarizer(imageRef) );
    Ref<BinaryBitmap> bb( new BinaryBitmap(binz) );

    DecodeHints hints;
    hints.setTryHarder(true);

    if(!decodeMulti){
        try {
            Ref<Result> res = reader->decode(bb, hints);
            results.push_back(res);
        }catch(zxing::Exception &e){
            qDebug()<<"decode error:"<<e.what();
        }
    }else{
        zxing::multi::GenericMultipleBarcodeReader _decoder(*reader);
        zxing::multi::GenericMultipleBarcodeReader* decoder = &_decoder;
        try {
            results = decoder->decodeMultiple(bb, hints);
        }catch(zxing::Exception &e){
            qDebug()<<"decode error:"<<e.what();
        }
    }
    return results;
}

static QRectF getRect(const ArrayRef<Ref<ResultPoint> > &resultPoints)
{
    if (resultPoints->size() < 2){
        qDebug()<<"no result point";
        return QRectF();
    }

    qreal xMin = resultPoints[0]->getX();
    qreal xMax = 0;
    qreal yMin = resultPoints[0]->getY();
    qreal yMax = 0;
    for (int i = 1; i < resultPoints->size(); ++i) {
        qreal x = resultPoints[i]->getX();
        if (x < xMin)
            xMin = x;
        if (x > xMax)
            xMax = x;
        qreal y = resultPoints[i]->getY();
        if (y < yMin)
            yMin = y;
        if (y > yMax)
            yMax = y;
    }
    qreal offset = (xMax - xMin) > (yMax - yMin) ?(xMax -xMin) :(yMax - yMin);
    offset /= 5;
    xMin -= offset;
    yMin -= offset;
    xMax += offset;
    yMax += offset;
    xMin = xMin > 0?xMin :0;
    yMin = yMin > 0?yMin :0;

    qreal w = xMax - xMin;
    qreal h = yMax - yMin;
    w = w < 1 ?1 :w;
    h = h < 1 ?1 :h;
    return QRectF(xMin, yMin, w, h);
}

enum DecoderFormat {
    DecoderFormat_None = 0,
    DecoderFormat_Aztec = 1 << 1,
    DecoderFormat_CODABAR = 1 << 2,
    DecoderFormat_CODE_39 = 1 << 3,
    DecoderFormat_CODE_93 = 1 << 4,
    DecoderFormat_CODE_128 = 1 << 5,
    DecoderFormat_DATA_MATRIX = 1 << 6,
    DecoderFormat_EAN_8 = 1 << 7,
    DecoderFormat_EAN_13 = 1 << 8,
    DecoderFormat_ITF = 1 << 9,
    DecoderFormat_MAXICODE = 1 << 10,
    DecoderFormat_PDF_417 = 1 << 11,
    DecoderFormat_QR_CODE = 1 << 12,
    DecoderFormat_RSS_14 = 1 << 13,
    DecoderFormat_RSS_EXPANDED = 1 << 14,
    DecoderFormat_UPC_A = 1 << 15,
    DecoderFormat_UPC_E = 1 << 16,
    DecoderFormat_UPC_EAN_EXTENSION = 1 << 17,
    DecoderFormat_CODE_128_GS1 = 1 << 18
} ;
QString decoderFormatToString(int fmt)
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

QList<DMResult> zxing_decode(const QImage& image ,int decodeMode ,bool decodeMulti)
{
    vector<Ref<Result> > results;
    switch (decodeMode) {
    case DECODETYPE_qrcode:{
        zxing::qrcode::QRCodeReader reader;
        results = qt_decode(image ,&reader ,decodeMulti);
    }
        break;
    case DECODETYPE_barcode:{
        zxing::oned::MultiFormatOneDReader reader(0);
        results = qt_decode(image ,&reader ,decodeMulti);
    }
        break;
    case DECODETYPE_allType:
    default:{
        zxing::MultiFormatReader reader;
        results = qt_decode(image ,&reader ,decodeMulti);
    }
        break;
    }

    QList<DMResult> resultList;
    struct DMResult result;
    QString string;
    int fmt;
    QString format;
    QString charSet;
//    QTextCodec *codec;
    for (auto res:results){

        string = QString(res->getText()->getText().c_str());
        if(string.isEmpty())
            continue;
        bool exist = false;
        for(auto result_already :resultList){
            if(result_already.text == string){
                exist = true;
            }
        }
        if(exist)
            continue;
        fmt = res->getBarcodeFormat().value;
        format = decoderFormatToString(1 << fmt);
        result.format = format;
        result.charSet = charSet;
        result.text = string;
        result.rect = getRect(res->getResultPoints());

        bool isurl = false;
        QString url = result.text.trimmed();
        if(!url.contains(" ")){
            QRegularExpression re_with_protocol("^[a-zA-Z][a-zA-Z0-9+-.]+:");
//                        QRegularExpression re_without_protocol("([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,}"
//                                                               +QString("(:\\d{1,5})?")
//                                                               +QString("(/|\\?|$)"));
            if(re_with_protocol.match(url).hasMatch())
                isurl = true;
//                        else if(re_without_protocol.match(url).hasMatch())
//                            isurl = true;
        }
        if(isurl)
            result.type = "URI";
        else
            result.type = "TEXT";

        resultList << result;
    }
    return resultList;
}
