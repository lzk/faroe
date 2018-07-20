#ifndef DECODE_H
#define DECODE_H

#include <QObject>
#include <QString>
#include <QRectF>
struct DMResult{
    QString text;
    QString type;
    QString fileName;
    QString format;
    QString charSet;
    qreal width;
    qreal height;
    QRectF rect;
};

struct DMDecodeResult{
    QString fileName;
    qreal width;
    qreal height;
    QList<DMResult> result;
};

typedef enum{
    DECODETYPE_qrcode,
    DECODETYPE_barcode,
    DECODETYPE_allType
}DECODETYPE_enum;

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
private:
    struct DMDecodeResult decode(const QString& fileName ,int decodeMode ,bool decodeMulti ,int angle = 2 ,int deep = 2);
    bool m_cancel;
};
#endif // DECODE_H
