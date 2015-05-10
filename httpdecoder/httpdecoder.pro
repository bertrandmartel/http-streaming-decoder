#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T21:57:26
#
#-------------------------------------------------

QT       -= gui

TARGET = httpdecoder
TEMPLATE = lib

DEFINES += HTTPDECODER_LIBRARY

DESTDIR = ./release

SOURCES += \
    protocol/inter/http/Httpconsumer.cpp \
    protocol/http/Httpdecoder.cpp \
    protocol/inter/http/Httpframe.cpp \
    protocol/http/Httpparser.cpp \
    utils/Stringutils.cpp

HEADERS +=\
    protocol/inter/http/Httpconsumer.h \
    protocol/http/Httpdecoder.h \
    protocol/http/Httpdecoder_global.h \
    protocol/inter/http/Httpframe.h \
    protocol/http/Httpparser.h \
    protocol/http/Httpstates.h \
    protocol/inter/http/IhttpFrame.h \
    utils/Stringutils.h \
    protocol/http/HttpDecoder.h \
    protocol/http/httpconstants.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
