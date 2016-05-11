#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T20:23:24
#
#-------------------------------------------------

QT       += core gui network opengl sql svg core serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += MAP_DEFAULT_TYPE_GOOGLE

TARGET = OpmapTest
TEMPLATE = app


SOURCES += main.cpp\
        testwindow.cpp \
    mapwidget.cpp \
    menu.cpp

HEADERS  += testwindow.h \
    mapwidget.h \
    menu.h

include(libs/opmapcontrol/opmapcontrol_external.pri)

INCLUDEPATH += libs/opmapcontrol

FORMS    += testwindow.ui

RESOURCES += \
    guiresource.qrc
