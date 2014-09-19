#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T15:55:35
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OCEmulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    emuscreen.cpp \
    compinstance.cpp

HEADERS  += mainwindow.h \
    emuscreen.h \
    compinstance.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../build-OCEmulator-Desktop-Debug/init.lua

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += lua
