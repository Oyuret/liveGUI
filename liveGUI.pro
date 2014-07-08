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
    streamitemwidget.cpp

HEADERS  += mainwindow.h \
    livestream.h \
    networkmanager.h \
    streamitemwidget.h \
    gamesitemdelegate.h

FORMS    += mainwindow.ui \
    streamitemwidget.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc

QMAKE_CFLAGS_RELEASE += /MT
QMAKE_CXXFLAGS_RELEASE += /MT
QMAKE_CFLAGS_DEBUG += /MTd
QMAKE_CXXFLAGS_DEBUG += /MTd
