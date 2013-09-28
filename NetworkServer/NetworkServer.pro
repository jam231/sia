QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wfloat-equal -Wundef \
                  -Wpointer-arith

QT += sql \
    network \
    widgets

INCLUDEPATH += \
    Utils

HEADERS += \
    server.h \
    market.h \
    connection.h \
    Utils/utilities.h \
    Utils/configmanager.h

SOURCES += \
    server.cpp \
    market.cpp \
    connection.cpp \
    main.cpp \
    Utils/utilities.cpp \
    Utils/configmanager.cpp


OTHER_FILES += \
    Files/config.ini


#DEFINES += QT_NO_DEBUG_OUTPUT

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/release/ -lNetworkProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/debug/ -lNetworkProtocol
else:unix: LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/ -lNetworkProtocol

INCLUDEPATH += $$PWD/../NetworkProtocol/NetworkProtocol
DEPENDPATH += $$PWD/../NetworkProtocol/NetworkProtocol
