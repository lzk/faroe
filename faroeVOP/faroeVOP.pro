QT += qml quick

CONFIG += c++11

SOURCES += \
            main.cpp \
            jkinterface.cpp \
    stringmodel.cpp \
            ../thumbnailviewer/imagemodel.cpp \
    ../lld/device.cpp \
    ../lld/scannerapi.cpp \
    ../lld/scanner.cpp \
    ../lld/deviceio.cpp \
    ../lld/ImgFile/Bmp.cpp \
    ../lld/ImgFile/ImgFile.cpp \
    ../lld/ImgFile/Jpeg.cpp \
    ../lld/ImgFile/Tiff.cpp \
    ../lld/platformapp.cpp \
    ../platform/devicemanager.cpp \
    ../platform/netio.cpp \
    ../platform/appqt.cpp \
    ../platform/netsnmp.cpp \
    ../platform/log.cpp \
    ../lld/setter.cpp \
    ../lld/setterapi.cpp

mac{
SOURCES += \
    ../platform/mac/mac_usb.cpp \
    ../platform/mac/usbio.cpp

HEADERS += \
    ../platform/mac/mac_usb.h \
    ../platform/mac/usbio.h
}

#lupdate_only{
#SOURCES += *.qml \
#    ../thumbnailviewer/*.qml
#}

HEADERS += \
    stringmodel.h \
    jkinterface.h \
    ../thumbnailviewer/imagemodel.h \
    ../lld/device.h \
    ../lld/type.h \
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
    ../lld/setterstruct.h

RESOURCES += qml.qrc \
    ../thumbnailviewer/thumbnailviewer.qrc \
    ../component/component.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
        *.qml \
        ../thumbnailviewer/*.qml \
        ../component/*.qml \

#QMAKE_CXXFLAGS += -Wmissing-field-initializers
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

