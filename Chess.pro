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
    gui/messagebox.h

SOURCES += \
    game/board.cpp \
    game/grid.cpp \
    game/figure.cpp \
    gui/graphicsview.cpp \
    gui/mainwindow.cpp \
    main.cpp \
    gui/sidebar.cpp \
    game/freefigures.cpp \
    gui/messagebox.cpp

