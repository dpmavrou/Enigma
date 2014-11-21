#-------------------------------------------------
#
# Project created by QtCreator 2014-11-03T04:21:06
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Enigma
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
        enigma_ecies.cpp \
    dialog.cpp \
    signindialog.cpp \
    email.cpp

HEADERS  += mainwindow.h \
        enigma_ecies.hpp \
        icryptosystem.hpp \
    dialog.h \
    signindialog.h \
    email.h

FORMS    += mainwindow.ui \
    dialog.ui \
    signindialog.ui \
    email.ui

LIBS += -lcrypto++ -lPocoNet -lPocoFoundation -lPocoNetSSL
