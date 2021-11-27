QT       += core gui

QT+= bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = B_t

TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp

HEADERS += mainwindow.h

FORMS   += mainwindow.ui

CONFIG  += mobility

MOBILITY =
