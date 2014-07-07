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
    gameitemwidget.cpp \
    gamesitemdelegate.cpp \
    networkmanager.cpp

HEADERS  += mainwindow.h \
    livestream.h \
    gameitemwidget.h \
    gamesitemdelegate.h \
    networkmanager.h

FORMS    += mainwindow.ui \
    gameitemwidget.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc
