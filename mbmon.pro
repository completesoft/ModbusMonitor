#-------------------------------------------------
#
# Project created by QtCreator 2017-10-18T20:16:38
#
#-------------------------------------------------

QT       += core gui charts serialbus sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mbmon
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialogsettemp.cpp \
    dialogsettings.cpp \
    dialogexport.cpp \
    database.cpp \
    log.cpp \
    controller.cpp \
    dialogsetpid.cpp \
    dialogarchive.cpp \
    controllerdata.cpp

HEADERS += \
        mainwindow.h \
    dialogsettemp.h \
    dialogsettings.h \
    dialogexport.h \
    database.h \
    log.h \
    controller.h \
    dialogsetpid.h \
    dialogarchive.h \
    controllerdata.h

FORMS += \
        mainwindow.ui \
    dialogsettemp.ui \
    dialogsettings.ui \
    dialogexport.ui \
    log.ui \
    dialogsetpid.ui \
    dialogarchive.ui

DISTFILES += \
    changelog.txt \
    mbmon.ini

VERSION = 1.4.0.0
