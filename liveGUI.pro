#-------------------------------------------------
#
# Project created by QtCreator 2014-07-04T19:50:19
#
#-------------------------------------------------

QT       += core gui network script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = liveGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    livestream.cpp \
    gamesitemdelegate.cpp \
    networkmanager.cpp \
    streamitemwidget.cpp \
    previewstreamwidget.cpp \
    twitchhandler.cpp \
    abstracthandler.cpp \
    favoriteitemwidget.cpp

HEADERS  += mainwindow.h \
    livestream.h \
    networkmanager.h \
    streamitemwidget.h \
    gamesitemdelegate.h \
    previewstreamwidget.h \
    twitchhandler.h \
    abstracthandler.h \
    global.h \
    favoriteitemwidget.h

FORMS    += mainwindow.ui \
    streamitemwidget.ui \
    previewstreamwidget.ui \
    favoriteitemwidget.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc

CONFIG += c++11
