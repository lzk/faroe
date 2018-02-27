#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T14:18:11
#
#-------------------------------------------------

QT       += core gui quick printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = faroeVOP
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        ../newui/jkinterface.cpp \
        ../newui/ImageViewer/imagemodel.cpp \
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
    ../imageManager/wrapperqimage.cpp

HEADERS += \
        mainwindow.h \
        ../newui/jkinterface.h \
        ../newui/jkenums.h \
        ../newui/ImageViewer/imagemodel.h \
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
    ../imageManager/wrapperqimage.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    ../newui/newui.qrc

#DEFINES += TEST

mac{
SOURCES += \
    ../platform/mac/mac_usb.cpp \
    ../platform/mac/usbio.cpp \
   ../platform/mac/mac_api.mm

OBJECTIVE_SOURCES +=
   ../platform/mac/mac_api.mm

HEADERS += \
    ../platform/mac/mac_usb.h \
    ../platform/mac/usbio.h

LIBS += -framework IOKit \
        -framework CoreFoundation \
        -framework Foundation \
         -L$$PWD/../../../../software/Qt5.9.1/5.9.1/clang_64/lib/ -lQt5Ftp \
         -lnetsnmp \
         -liconv

#         `net-snmp-config --cflags`

INCLUDEPATH += $$PWD/../../../3rdparty/install/include
DEPENDPATH += $$PWD/../../../3rdparty/install/include
PRE_TARGETDEPS += $$PWD/../../../3rdparty/install/lib/libzxing.a

  QMAKE_INFO_PLIST = ../mac/Info.plist
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../3rdparty/install/lib/release/ -lzxing
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../3rdparty/install/lib/debug/ -lzxing
else:unix: LIBS += -L$$PWD/../../../3rdparty/install/lib/ -lzxing

