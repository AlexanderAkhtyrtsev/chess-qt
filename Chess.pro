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

INCLUDEPATH += $$PWD/include
HEADERS += $$PWD/include/*.h
SOURCES += $$PWD/src/*.cpp

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/values/libs.xml

win32 {
    RC_ICONS = img/icon.ico
#	QMAKE_LFLAGS_RELEASE += -static -static-libgcc
}

CONFIG(debug, release|debug):DEFINES += _DEBUG

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
