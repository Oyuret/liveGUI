#-------------------------------------------------
#
# Project created by QtCreator 2014-07-04T19:50:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = liveGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    livestream.cpp \
    gameitemwidget.cpp

HEADERS  += mainwindow.h \
    livestream.h \
    gameitemwidget.h

FORMS    += mainwindow.ui \
    gameitemwidget.ui

OTHER_FILES += \
    icons/play-disabled.png \
    icons/play.png \
    icons/stop-disabled.png \
    icons/stop.png \
    icons/favorite.png \
    icons/favorites.png

RESOURCES += \
    icons.qrc
