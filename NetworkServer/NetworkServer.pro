QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wundef \
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
    Utils/utils.h

SOURCES += \
    server.cpp \
    market.cpp \
    connection.cpp \
    main.cpp


OTHER_FILES += \
    Files/config.ini


#DEFINES += QT_NO_DEBUG_OUTPUT

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/release/ -lNetworkProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/debug/ -lNetworkProtocol
else:unix: LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/ -lNetworkProtocol

INCLUDEPATH += $$PWD/../NetworkProtocol/NetworkProtocol
DEPENDPATH += $$PWD/../NetworkProtocol/NetworkProtocol

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Utilities/Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Utilities/Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../Utilities/Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../Utilities/Utilities
DEPENDPATH += $$PWD/../Utilities/Utilities

gcc:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Utilities/Utilities/release/libUtilities.a
else:gcc:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Utilities/Utilities/debug/libUtilities.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Utilities/Utilities/libUtilities.a
