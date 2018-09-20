//
//  zxingobjc_decode.m
//  faroeVOP
//
//  Created by Jacky Liang on 2018/7/16.
//

#import <Foundation/Foundation.h>
#import <ZXingObjC.h>

#include "decodemanager.h"
#include <QImage>
#include <QRectF>
#include <QDebug>
static NSMutableArray* OCdecode(const uchar* bytes ,int pixelsWidth ,int pixelsHight ,id<ZXReader> reader ,bool decodeMulti)
{
    NSError *error;
    NSMutableArray* results = [NSMutableArray array];
//    @autoreleasepool {

        ZXRGBLuminanceSource* source = [[ZXRGBLuminanceSource alloc] initWithWidth:pixelsWidth height:pixelsHight pixels:((int*) bytes) pixelsLen:pixelsWidth*pixelsHight];
        ZXHybridBinarizer *binarizer = [[ZXHybridBinarizer alloc] initWithSource: source];
        ZXBinaryBitmap *bitmap = [[ZXBinaryBitmap alloc] initWithBinarizer:binarizer];

        ZXDecodeHints *hints = [ZXDecodeHints hints];
        hints.tryHarder = true;
        if(!decodeMulti){
            ZXResult * rst = [reader decode:bitmap hints:hints error:&error];
            if(rst != nil){
                [results addObject:rst];
            }
        }else{
            //            ZXMultiFormatReader* reader = [[ZXMultiFormatReader alloc] init];
            ZXGenericMultipleBarcodeReader* multiReader = [[ZXGenericMultipleBarcodeReader alloc] initWithDelegate:reader];
            results = [[multiReader decodeMultiple:bitmap hints:hints error:&error] mutableCopy];
            //                results = [[multiReader decodeMultiple:bitmap error:&error] mutableCopy];
        }
//    }
    return results;
}

static QRectF getRect(ZXResult* result)
{
    float xMin =  NSIntegerMax;
    float yMin = NSIntegerMax;
    float xMax = 0.0f;
    float yMax = 0.0f;
    for (ZXResultPoint *point in result.resultPoints) {
        if ((id)point == [NSNull null]) {
            continue;
        }
        float x = [point x];
        float y = [point y];
        if (x < xMin) {
            xMin = x;
        }
        if (y < yMin) {
            yMin = y;
        }
        if (x > xMax) {
            xMax = x;
        }
        if (y > yMax) {
            yMax = y;
        }
    }
    //    NSLog("xMin:%f,yMin:%f,xMax:%f,yMax:%f",xMin,yMin,xMax,yMax);

    CGFloat offset = (xMax - xMin) > (yMax - yMin) ?(xMax -xMin) :(yMax - yMin);
    offset /= 5;
    xMin -= offset;
    yMin -= offset;
    xMax += offset;
    yMax += offset;
    xMin = xMin > 0?xMin :0;
    yMin = yMin > 0?yMin :0;

    CGFloat w = xMax - xMin;
    CGFloat h = yMax - yMin;
    w = w < 1 ?1 :w;
    h = h < 1 ?1 :h;
    return QRectF(xMin, yMin, w, h);
}

QString barcodeFormatToString(ZXBarcodeFormat format)
{
    switch (format) {
        case kBarcodeFormatAztec:               return "Aztec";
        case kBarcodeFormatCodabar:             return "CODABAR";
        case kBarcodeFormatCode39:              return "Code 39";
        case kBarcodeFormatCode93:              return "Code 93";
        case kBarcodeFormatCode128:             return "Code 128";
        case kBarcodeFormatDataMatrix:          return "Data Matrix";
        case kBarcodeFormatEan8:                return "EAN-8";
        case kBarcodeFormatEan13:               return "EAN-13";
        case kBarcodeFormatITF:                 return "ITF";
        case kBarcodeFormatPDF417:              return "PDF417";
        case kBarcodeFormatQRCode:              return "QR Code";
        case kBarcodeFormatRSS14:               return "RSS 14";
        case kBarcodeFormatRSSExpanded:         return "RSS Expanded";
        case kBarcodeFormatUPCA:                return "UPCA";
        case kBarcodeFormatUPCE:                return "UPCE";
        case kBarcodeFormatUPCEANExtension:     return "UPC/EAN extension";
        case kBarcodeFormatMaxiCode:            return "MaxiCode";
        default:                                return "Unknown";
    }
}

QString resultType(ZXParsedResultType type)
{
    switch (type) {
        case kParsedResultTypeAddressBook:              return "ADDRESSBOOK";
        case kParsedResultTypeEmailAddress:             return "EMAIL_ADDRESS";
        case kParsedResultTypeProduct:                  return "PRODUCT";
        case kParsedResultTypeURI:                      return "URI";
        case kParsedResultTypeText:                     return "TEXT";
        case kParsedResultTypeAndroidIntent:            return "AndroidIntent";
        case kParsedResultTypeGeo:                      return "GEO";
        case kParsedResultTypeTel:                      return "TEL";
        case kParsedResultTypeSMS:                      return "SMS";
        case kParsedResultTypeCalendar:                 return "CALENDAR";
        case kParsedResultTypeWifi:                     return "WIFI";
        case kParsedResultTypeNDEFSMartPoster:          return "NDEFSMartPoster";
        case kParsedResultTypeMobiletagRichWeb:         return "MobiletagRichWeb";
        case kParsedResultTypeISBN:                     return "ISBN";
        case kParsedResultTypeVIN:                      return "VIN";
        default:                                        return "Unknown";
    }
}

QList<DMResult> zxing_decode(const QImage& image ,int decodeMode ,bool decodeMulti)
{
    NSMutableArray* results = [NSMutableArray array];
    @autoreleasepool {
        id<ZXReader> reader;
        switch (decodeMode) {
        case DECODETYPE_qrcode:
            reader = [[ZXQRCodeReader alloc] init];
            break;
        case DECODETYPE_barcode:
            reader = [[ZXMultiFormatOneDReader alloc] initWithHints:nil];
            break;
        case DECODETYPE_allType:
        default:
            reader = [[ZXMultiFormatReader alloc] init];
            break;
        }
        QImage aaa = image;
        if(image.depth() != 32){
            aaa = image.convertToFormat(QImage::Format_RGB32);
        }
        int pixelsWidth = aaa.width();
        int pixelsHight = aaa.height();
        const uchar* bytes = aaa.constBits();
        results = [OCdecode(bytes ,pixelsWidth ,pixelsHight ,reader ,decodeMulti) mutableCopy];
    }
    QList<DMResult> resultList;
    struct DMResult result;
    QString string;
    for (ZXResult* res in results) {
        if([res.text length] <= 0)
            continue;
        string = QString::fromNSString(res.text);
        bool exist = false;
        for(auto result_already :resultList){
            if(result_already.text == string){
                exist = true;
            }
        }
        if(exist)
            continue;
        result.format = barcodeFormatToString(res.barcodeFormat);
        result.text = string;
        ZXParsedResult* parsedResult = [ZXResultParser parseResult:res];
        result.type = resultType(parsedResult.type);
        result.rect = getRect(res);
        resultList << result;
    }
    return resultList;
}
