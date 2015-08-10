#-------------------------------------------------
#
# Project created by QtCreator 2015-08-10T14:32:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-mapper-tagger
TEMPLATE = app

LIBS += -L/usr/local/lib -lgit2

SOURCES += main.cpp\
        mainwindow.cpp \
    listviewtab.cpp \
    textviewtab.cpp \
    linkview.cpp \
    signallist.cpp \
    gitinterface.cpp

HEADERS  += mainwindow.h \
    listviewtab.h \
    textviewtab.h \
    linkview.h \
    signallist.h \
    gitinterface.h

FORMS    += mainwindow.ui \
    linkview.ui \
    signallist.ui
