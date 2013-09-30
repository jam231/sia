QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra  -Wfloat-equal -Wundef \
                  -Wpointer-arith

TARGET = NetworkProtocol_TEST

QT       += network testlib

QT       -= gui

CONFIG += console
CONFIG += testcase
CONFIG -= app_bundle

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    main.cpp \
    messagetest.cpp

HEADERS += \
    messagetest.h

INCLUDEPATH += \
 ../NetworkProtocol/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/release/ -lNetworkProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/debug/ -lNetworkProtocol
else:unix: LIBS += -L$$OUT_PWD/../NetworkProtocol/ -lNetworkProtocol

INCLUDEPATH += $$PWD/../NetworkProtocol
DEPENDPATH += $$PWD/../NetworkProtocol
