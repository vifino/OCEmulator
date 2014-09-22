#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T15:55:35
#
# Created By: Altenius and vifino
#
#-------------------------------------------------

QT       += core gui opengl qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = OCEmulator

INCLUDEPATH += freetype-gl

TEMPLATE = subdirs
CONFIG += link_pkgconfig
CONFIG += ordered
SUBDIRS += freetype-gl src
