#-------------------------------------------------
#
# Project created by QtCreator 2016-12-25T17:30:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotDataCollect
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

PROJNAME = RobotDataCollect
INSTTYPE = APP
include(RobotSDK_Main.pri)