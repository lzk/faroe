QT += qml quick

TARGET  = ScaledImage
TEMPLATE = app

SOURCES += \
            main.cpp \
           ../thumbnailviewer/imageresponseprovider.cpp \
    ../thumbnailviewer/imagemodel.cpp \
    jkinterface.cpp

lupdate_only{
SOURCES += *.qml \
    ../thumbnailviewer/*.qml
}

RESOURCES += \
    scalesimage.qrc \
    ../thumbnailviewer/thumbnailviewer.qrc \
    ../component/component.qrc

HEADERS += \
    ../thumbnailviewer/imageresponseprovider.h \
    ../thumbnailviewer/imagemodel.h \
    jkinterface.h

