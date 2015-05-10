#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T21:57:26
#
#-------------------------------------------------

QT       -= gui

TARGET = httpdecoder
TEMPLATE = lib

DEFINES += HTTPDECODER_LIBRARY

SOURCES += \
    Httpconsumer.cpp \
    Httpdecoder.cpp \
    Httpframe.cpp \
    Httpparser.cpp \
    Stringutils.cpp

HEADERS +=\
    Httpconsumer.h \
    Httpdecoder.h \
    Httpdecoder_global.h \
    Httpframe.h \
    Httpparser.h \
    Httpstates.h \
    IhttpFrame.h \
    Stringutils.h \
    HttpDecoder.h \
    httpconstants.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
