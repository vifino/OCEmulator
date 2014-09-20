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
    compinstance.cpp \
    component.cpp \
    filesystemcomponent.cpp

HEADERS  += mainwindow.h \
    emuscreen.h \
    compinstance.h \
    component.h \
    filesystemcomponent.h \
    lua.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
    init.lua

unix|win32: LIBS += -lboost_system

unix|win32: LIBS += -llua5.2

unix|win32: LIBS += -lboost_filesystem
