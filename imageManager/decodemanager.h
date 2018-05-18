#ifndef DECODEMANAGER_H
#define DECODEMANAGER_H

#include <QObject>
#include <QImage>
namespace zxing {
class Reader;
class DecodeHints;
}
struct DMResult{
    QString text;
    QString fileName;
    QString format;
    QString charSet;
    qreal width;
    qreal height;
};

struct DMDecodeResult{
    QString fileName;
    qreal width;
    qreal height;
    QList<DMResult> result;
};
class DecodeManager : public QObject
{
    Q_OBJECT
public:
    explicit DecodeManager(QObject *parent = nullptr);

    QString decodeOneD(const QString& fileName);
    struct DMDecodeResult decodeMultiQrcode(const QString& fileName);
    struct DMDecodeResult decodeMultiOneD(const QString& fileName);
    struct DMDecodeResult decodeMultiAll(const QString& fileName);

    void cancel(bool iscancel);
public:
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
public slots:

private:
    struct DMDecodeResult decodeMulti(const QString& fileName ,zxing::Reader* ,zxing::DecodeHints hints ,int angle = 2);
    struct DMDecodeResult decode(const QString& fileName ,zxing::Reader* ,zxing::DecodeHints hints ,int angle = 2);

    static QString decoderFormatToString(int fmt);
    bool m_cancel;
};

#endif // DECODEMANAGER_H
