#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T15:55:35
#
# Created By: Altenius and vifino
#
#-------------------------------------------------

QT       += core gui opengl qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OCEmulator
TEMPLATE = app
CONFIG += link_pkgconfig
CONFIG += ordered

SOURCES += main.cpp\
        mainwindow.cpp \
    emuscreen.cpp \
    compinstance.cpp \
    component.cpp \
    filesystemcomponent.cpp \
    screencomponent.cpp

HEADERS  += mainwindow.h \
    emuscreen.h \
    compinstance.h \
    component.h \
    filesystemcomponent.h \
    lua.hpp \
    screencomponent.h

FORMS    += mainwindow.ui \
    mainwindow.ui

OTHER_FILES += \
    init.lua

INCLUDEPATH += $$quote(/usr/include/freetype2)

unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -llua5.2
unix|win32: LIBS += -licuuc
unix|win32: LIBS += -lboost_filesystem
unix|win32: LIBS += -lfreetype -lftgl

RESOURCES += \
    fonts.qrc
