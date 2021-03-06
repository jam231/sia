QMAKE_CXXFLAGS += -flto -std=c++11 -Wall -Wextra -Wundef \
                  -Wpointer-arith

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QT       -= gui

TARGET = Utilities
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    writer.cpp \
    logger.cpp \
    configmanager.cpp \
    utilities.cpp

HEADERS += \
    writer.h \
    logger.h \
    configmanager.h \
    utilities.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
