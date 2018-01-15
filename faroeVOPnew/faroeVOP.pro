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
    ../imageManager/imagemanager.cpp

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
    ../imageManager/imagemanager.h

mac{
SOURCES += \
    ../platform/mac/mac_usb.cpp \
    ../platform/mac/usbio.cpp

HEADERS += \
    ../platform/mac/mac_usb.h \
    ../platform/mac/usbio.h
}

FORMS += \
        mainwindow.ui

RESOURCES += \
    ../newui/newui.qrc

DEFINES += TEST

windows{
#LIBS += -LC:/usr/lib/ -lnetsnmp
#INCLUDEPATH += C:/usr/include
SOURCES -= ../platform/netsnmp.cpp
DEFINES += OS_WIN
}else{
LIBS += `net-snmp-config --cflags` -lnetsnmp
}

mac: LIBS += -framework IOKit
mac: LIBS += -framework CoreFoundation

