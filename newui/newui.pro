TEMPLATE = app

QT += qml quick printsupport
CONFIG += c++11

TARGET = faroeVOP
VERSION = 0.0.13

#UI_HEADERS_DIR = ../build
#UI_SOURCES_DIR = ../build
#UI_DIR = ../build
#MOC_DIR = ../build
#RCC_DIR = ../build
#CONFIG(release){
#OBJECTS_DIR = ../build/release
#DESTDIR = ../build/release
#}
#CONFIG(debug){
#OBJECTS_DIR = ../build/debug
#DESTDIR = ../build/debug
#}

SOURCES += main.cpp \
            jkinterface.cpp \
    ImageViewer/imagemodel.cpp \
    ../lld/device.cpp \
    ../lld/scannerapi.cpp \
    ../lld/scanner.cpp \
    ../lld/ImgFile/Bmp.cpp \
    ../lld/ImgFile/ImgFile.cpp \
    ../lld/ImgFile/Jpeg.cpp \
    ../lld/ImgFile/Tiff.cpp \
    ../platform/devicemanager.cpp \
    ../platform/netio.cpp \
    ../platform/appqt.cpp \
    ../platform/netsnmp.cpp \
    ../platform/log.cpp \
    ../lld/setter.cpp \
    ../lld/setterapi.cpp \
    ../imageManager/imagemanager.cpp \
    ../imageManager/decodemanager.cpp \
    ../platform/mac/macapi.cpp \
    ../platform/platform.cpp \
    ../imageManager/wrapperqimage.cpp \
    ../lld/Calibration/calibration_flow.cpp \
    ../lld/Calibration/calibration_scan.cpp \
    ../lld/Calibration/EdgeDetect.cpp \
    ../lld/Calibration/MotorMove.cpp \
    ../lld/Calibration/ScanCMD.cpp \
    ../lld/Calibration/ScanFunction.cpp \
    ../lld/Calibration/macusbio.cpp \
    worker.cpp \
    ../functions/imagefunctions.cpp

HEADERS += \
    jkenums.h \
    jkinterface.h \
    ImageViewer/imagemodel.h \
    ../lld/device.h \
    ../lld/scannerstruct.h \
    ../lld/scannerapi.h \
    ../lld/scanner.h \
    ../lld/deviceio.h \
    ../lld/ImgFile/ImgFile.h \
    ../lld/platformapp.h \
    ../platform/devicemanager.h \
    ../platform/netio.h \
    ../platform/appqt.h \
    ../platform/log.h \
    ../platform/version.h \
    ../lld/setter.h \
    ../lld/setterapi.h \
    ../lld/setterstruct.h \
    ../platform/devicestruct.h \
    ../imageManager/imagemanager.h \
    ../imageManager/decodemanager.h \
    ../platform/platform.h \
    ../imageManager/wrapperqimage.h \
    ../lld/Calibration/EdgeDetect.h \
    ../lld/Calibration/model.h \
    ../lld/Calibration/ScanCMD.h \
    ../lld/Calibration/macusbio.h \
    ../lld/ImgFile/utype.h \
    worker.h \
    ../functions/imagefunctions.h

RESOURCES += \
    ../newui/newui.qrc

#DEFINES += TEST

mac{
SOURCES += \
    ../platform/mac/mac_usb.cpp \
    ../platform/mac/usbio.cpp \
   ../platform/mac/mac_api.mm \
    ../platform/mac/MacDelegate.mm

#OBJECTIVE_SOURCES +=
#   ../platform/mac/mac_api.mm \

HEADERS += \
    ../platform/mac/mac_usb.h \
    ../platform/mac/usbio.h \
    ../platform/mac/MacDelegate.h

LIBS += -framework IOKit \
        -framework CoreFoundation \
        -framework Foundation \
         -L$$PWD/../qtftp/build/Release/lib/ -lQt5Ftp \
         -lnetsnmp \
         -liconv

#         `net-snmp-config --cflags`

INCLUDEPATH += $$PWD/../qtftp/build/Release/include

INCLUDEPATH += $$PWD/../libs/zxing-cpp/include
DEPENDPATH += $$PWD/../libs/zxing-cpp/include
LIBS += $$PWD/../libs/zxing-cpp/lib/libzxing.a
#PRE_TARGETDEPS += $$PWD/libs/zxing-cpp/lib/libzxing.a

    ICON = faroeVOP.icns
#    ICON = ../../mac/faroeVOP.icns
#    QMAKE_INFO_PLIST = ../mac/Info.plist
}else{
    windows{
    #LIBS += -LC:/usr/lib/ -lnetsnmp
    #INCLUDEPATH += C:/usr/include
    SOURCES -= ../platform/netsnmp.cpp
    DEFINES += OS_WIN
    LIBS += -lQt5Ftp
    }
}

TRANSLATIONS = ../newui/translations/vop.en.ts  \
                ../newui/translations/vop.zh_CN.ts
lupdate_only{
SOURCES += ../newui/*.qml \
            ../newui/*.js \
          ../newui/component/*.qml \
          ../newui/component/*.js \
          ../newui/component/path/*.qml \
          ../newui/component/path/*.js \
          ../newui/ImageViewer/*.qml \
          ../newui/ImageViewer/*.js \
          ../newui/ScanPage/*.qml \
          ../newui/ScanPage/*.js \
          ../newui/ScanToPage/*.qml \
          ../newui/ScanToPage/*.js \
          ../newui/SettingsPage/*.qml \
          ../newui/SettingsPage/*.js \
          ../newui/SettingsPage/component/*.qml \
          ../newui/SettingsPage/component/*.js \
          ../newui/SettingsPage/QuickScanSettings/*.qml \
          ../newui/SettingsPage/QuickScanSettings/*.js \
          ../newui/SettingsPage/ScanSetting/*.qml \
          ../newui/SettingsPage/ScanSetting/*.js \
}

DISTFILES += \
    translations/vop.en.qm \
    translations/vop.zh_CN.qm

