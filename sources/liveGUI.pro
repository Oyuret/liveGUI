#-------------------------------------------------
#
# Project created by QtCreator 2014-07-04T19:50:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = liveGUI
TEMPLATE = app


SOURCES += \
    src/abstracthandler.cpp \
    src/favoriteitemwidget.cpp \
    src/favoritewidget.cpp \
    src/game.cpp \
    src/gamesitemdelegate.cpp \
    src/gameswidget.cpp \
    src/livestream.cpp \
    src/mainwindow.cpp \
    src/networkmanager.cpp \
    src/previewstreamwidget.cpp \
    src/service.cpp \
    src/stream.cpp \
    src/streamitemwidget.cpp \
    src/streamswidget.cpp \
    src/twitchhandler.cpp \
    main.cpp \
    src/browsingwidget.cpp \
    src/remoteplayer.cpp

HEADERS  += \
    include/abstracthandler.h \
    include/favoriteitemwidget.h \
    include/favoritewidget.h \
    include/game.h \
    include/gamesitemdelegate.h \
    include/gameswidget.h \
    include/livestream.h \
    include/mainwindow.h \
    include/networkmanager.h \
    include/previewstreamwidget.h \
    include/service.h \
    include/stream.h \
    include/streamitemwidget.h \
    include/streamswidget.h \
    include/twitchhandler.h \
    include/browsingwidget.h \
    include/remoteplayer.h

FORMS    += \
    ui/favoriteitemwidget.ui \
    ui/favoritewidget.ui \
    ui/gameswidget.ui \
    ui/mainwindow.ui \
    ui/previewstreamwidget.ui \
    ui/streamitemwidget.ui \
    ui/streamswidget.ui \
    ui/browsingwidget.ui

OTHER_FILES +=

RESOURCES += \
    qrc/icons.qrc

CONFIG += c++11

INCLUDEPATH += include

Release:DESTDIR     = release
Release:OBJECTS_DIR = release/build/obj
Release:MOC_DIR     = release/build/moc
Release:RCC_DIR     = release/build/rcc
Release:UI_DIR      = release/build/ui

Debug:DESTDIR       = debug
Debug:OBJECTS_DIR   = debug/build/obj
Debug:MOC_DIR       = debug/build/moc
Debug:RCC_DIR       = debug/build/rcc
Debug:UI_DIR        = debug/build/ui
