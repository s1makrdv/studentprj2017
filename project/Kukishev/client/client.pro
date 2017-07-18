QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += $$PWD/../common

TEMPLATE = app

SOURCES += main.cpp \
    Worker.cpp \
    Client.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    ClientChat.cpp \
    MessageManager.cpp

HEADERS += \
    Worker.h \
    Client.h \
    ../common/define.h \
    ../common/Helper.h \
    ClientChat.h \
    MessageManager.h

