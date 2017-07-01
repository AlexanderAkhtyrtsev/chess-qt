#-------------------------------------------------
#
# Project created by QtCreator 2017-05-12T21:54:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grid.cpp \
    figure.cpp \
    board.cpp \
    graphicsview.cpp

HEADERS  += mainwindow.h \
    grid.h \
    board.h \
    graphicsview.h \
    figure.h

OTHER_FILES +=

RESOURCES += \
    res.qrc

