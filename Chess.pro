#-------------------------------------------------
#
# Project created by QtCreator 2017-05-12T21:54:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


RESOURCES += \
    res.qrc

HEADERS += \
    game/board.h \
    game/figure.h \
    game/grid.h \
    gui/graphicsview.h \
    gui/mainwindow.h \
    gui/sidebar.h \
    game/freefigures.h \
    gui/messagebox.h \
    gui/dialbox.h \
    gui/fader.h \
    gui/window.h \
    gui/optionswindow.h \
    gui/helpwnd.h \
    game/gametimer.h

SOURCES += \
    game/board.cpp \
    game/grid.cpp \
    game/figure.cpp \
    gui/graphicsview.cpp \
    gui/mainwindow.cpp \
    main.cpp \
    gui/sidebar.cpp \
    game/freefigures.cpp \
    gui/messagebox.cpp \
    gui/dialbox.cpp \
    gui/fader.cpp \
    gui/window.cpp \
    gui/optionswindow.cpp \
    gui/helpwnd.cpp \
    game/gametimer.cpp

DISTFILES += \
    android/AndroidManifest.xml

win32 {
    RC_ICONS = img/icon.ico
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
