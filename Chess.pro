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
    game/grid.h \
    gui/graphicsview.h \
    gui/mainwindow.h \
    gui/window.h \
    game/gametimer.h \
    game/piece.h \
    game/freepieces.h

SOURCES += \
    game/board.cpp \
    game/grid.cpp \
    gui/graphicsview.cpp \
    gui/mainwindow.cpp \
    main.cpp \
    gui/window.cpp \
    game/gametimer.cpp \
    game/piece.cpp \
    game/freepieces.cpp

DISTFILES += \
    android/AndroidManifest.xml

win32 {
    RC_ICONS = img/icon.ico
#	QMAKE_LFLAGS_RELEASE += -static -static-libgcc
}
CONFIG(debug, release|debug):DEFINES += _DEBUG

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
