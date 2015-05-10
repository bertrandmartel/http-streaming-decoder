#-------------------------------------------------
#
# Project created by QtCreator 2015-05-09T20:50:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = httpdecoder-test

DEPENDPATH += . ../httpdecoder
INCLUDEPATH += ../httpdecoder
LIBS += -L../httpdecoder -lhttpdecoder

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    launcher.cpp

HEADERS += \
    launcher.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../httpdecoder/release/release/ -lhttpdecoder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../httpdecoder/release/debug/ -lhttpdecoder
else:unix: LIBS += -L$$PWD/../httpdecoder/release/ -lhttpdecoder

INCLUDEPATH += $$PWD/../httpdecoder/release
DEPENDPATH += $$PWD/../httpdecoder/release
