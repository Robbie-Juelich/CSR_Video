#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T19:51:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4){  ### qt version is 5
    QT += widgets
} else {
    QT += qt3support      ### qt version is 4  for linux with gstreamer 1.0
}

TARGET = VideoManager
TEMPLATE = app

CONFIG += silent

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build

SOURCES += main.cpp\
        mainwindow.cpp \
        switchcaps.cpp \
        videoplayer.cpp \
        audioplayer.cpp \
    udpregister.cpp \
    udpheartbeat.cpp \
    outsocket.cpp \
    sendmsg.cpp \
    audioplayertrain.cpp

HEADERS  += mainwindow.h \
                switchcaps.h \
            videoplayer.h \
            audioplayer.h \
    udpregister.h \
    msg.h \
    udpheartbeat.h \
    outsocket.h \
    socketports.h \
    sendmsg.h \
    audioplayertrain.h \
    config.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc


include(qtgstreamer.pri)
include(qslog/QsLog.pri)
include(qtsingleapplication/qtsingleapplication.pri)

RESOURCES += \
    res.qrc
