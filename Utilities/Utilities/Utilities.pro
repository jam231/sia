QMAKE_CXXFLAGS += -std=c++0x -Wall  -Werror -Wextra -Wundef \
                  -Wpointer-arith

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
