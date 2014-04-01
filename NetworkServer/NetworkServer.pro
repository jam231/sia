QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wundef \
                  -Wpointer-arith

QT += sql \
    network \
    widgets

INCLUDEPATH += \
    Utils

HEADERS += \
    #server.h \
    #market.h \
    #connection.h \
    Utils/utils.h \
    DataStorage/datahub.h \
    DataStorage/datastorage.h \
    Servers/tradingserver.h \
    Servers/loginserver.h \
    master.h \
    Utils/scheduling.h \
    Servers/eventserver.h \
    Servers/connections.h \
    DataStorage/postgresdatastorage.h

SOURCES += \
    #server.cpp \
    #market.cpp \
    #connection.cpp \
    main.cpp \
    DataStorage/datahub.cpp \
    DataStorage/datastorage.cpp \
    Servers/tradingserver.cpp \
    Servers/loginserver.cpp \
    Servers/eventserver.cpp \
    master.cpp \
    Utils/scheduling.cpp \
    Servers/connections.cpp \
    Utils/utils.cpp \
    DataStorage/postgresdatastorage.cpp


OTHER_FILES += \
    Files/config.ini


install_settings.path = $$OUT_PWD/Files/
install_settings.files = $$PWD/Files/config.ini

INSTALLS += install_settings
#DEFINES += QT_NO_DEBUG_OUTPUT

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/release/ -lNetworkProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/debug/ -lNetworkProtocol
else:unix: LIBS += -L$$OUT_PWD/../NetworkProtocol/NetworkProtocol/ -lNetworkProtocol

INCLUDEPATH += $$PWD/../NetworkProtocol/NetworkProtocol
DEPENDPATH += $$PWD/../NetworkProtocol/NetworkProtocol

win32:gcc:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Utilities/Utilities/release/ -lUtilities
else:win32:gcc:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Utilities/Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../Utilities/Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../Utilities/Utilities
DEPENDPATH += $$PWD/../Utilities/Utilities

win32:gcc:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Utilities/Utilities/release/libUtilities.a
else:win32:gcc:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Utilities/Utilities/debug/libUtilities.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Utilities/Utilities/libUtilities.a
