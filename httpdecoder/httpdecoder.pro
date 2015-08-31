#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T21:57:26
#
#-------------------------------------------------
QT       -= gui

TARGET = httpdecoder
TEMPLATE = lib
CONFIG += staticlib

DEFINES += HTTPDECODER_LIBRARY

DESTDIR = release

OBJECTS_DIR=bin

SOURCES += \
    protocol/http/httpdecoder.cpp \
    protocol/http/httpparser.cpp \
    protocol/inter/http/httpconsumer.cpp \
    protocol/inter/http/httpframe.cpp \
    utils/stringutils.cpp

HEADERS +=\
    protocol/http/Httpdecoder.h \
    protocol/inter/http/IhttpFrame.h \
    protocol/http/httpconstants.h \
    protocol/http/httpdecoder.h \
    protocol/http/httpparser.h \
    protocol/http/httpstates.h \
    protocol/inter/http/httpconsumer.h \
    protocol/inter/http/httpframe.h \
    utils/stringutils.h \
    protocol/http/httpdecoder_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

QMAKE_CLEAN += -r $${PWD}/$${DESTDIR}

QMAKE_POST_LINK +=$$quote(rsync -avm --include=*/ --include=*.h --exclude=* $${PWD}/$${SOURCES_DIR}/ $${PWD}/$${DESTDIR})
