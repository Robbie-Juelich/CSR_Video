#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T10:53:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocalManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    msg.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build


