#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T21:44:28
#
#-------------------------------------------------

QT       += core gui network

TARGET = DRC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dcommunication.cpp

HEADERS  += mainwindow.h \
    dcommunication.h \
    dabstractsign.h \
    dabstracthash.h \
    dsockets.h

FORMS    += mainwindow.ui
