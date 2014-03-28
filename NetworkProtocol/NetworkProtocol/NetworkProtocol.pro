QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    Responses/lasttransactionmsg.cpp \
    Responses/showbestordermsg.cpp \
    Responses/response.cpp \
    Responses/registerusersuccessmsg.cpp \
    Responses/okmsg.cpp \
    Responses/failuremsg.cpp \
    DataTransferObjects/types.cpp \
    DataTransferObjects/order.cpp \
    DataTransferObjects/lasttransaction.cpp \
    DataTransferObjects/bestorder.cpp \
    Requests/unsubscribestockmsg.cpp \
    Requests/subscribestockmsg.cpp \
    Requests/sellstockmsg.cpp \
    Requests/request.cpp \
    Requests/registerusermsg.cpp \
    Requests/loginusermsg.cpp \
    Requests/getstockinfomsg.cpp \
    Requests/getmystocksmsg.cpp \
    Requests/getmyordersmsg.cpp \
    Requests/cancelordermsg.cpp \
    Requests/buystockmsg.cpp \
    networkprotocol_utilities.cpp \
    Responses/orderacceptedmsg.cpp \
    Responses/shownewordermsg.cpp \
    Responses/listofstocksmsg.cpp \
    Responses/stockinfomsg.cpp \
    Responses/listofordersmsg.cpp \
    Requests/getcompanystatusmsg.cpp \
    Responses/companystatusmsg.cpp \
    Responses/orderchangemsg.cpp \
    Responses/ordercompletedmsg.cpp \
    message.cpp \
    Responses/shownobestordermsg.cpp \
    DataTransferObjects/stockinfo.cpp \
    DataTransferObjects/userstocks.cpp




HEADERS += \
    networkprotocol_global.h \
    message.h \
    Responses/showbestordermsg.h \
    Responses/response.h \
    Responses/registerusersuccessmsg.h \
    Responses/okmsg.h \
    Responses/failuremsg.h \
    DataTransferObjects/types.h \
    DataTransferObjects/order.h \
    DataTransferObjects/lasttransaction.h \
    DataTransferObjects/bestorder.h \
    Requests/unsubscribestockmsg.h \
    Requests/subscribestockmsg.h \
    Requests/sellstockmsg.h \
    Requests/request.h \
    Requests/registerusermsg.h \
    Requests/loginusermsg.h \
    Requests/getstockinfomsg.h \
    Requests/getmystocksmsg.h \
    Requests/getmyordersmsg.h \
    Requests/cancelordermsg.h \
    Requests/buystockmsg.h \
    networkprotocol_utilities.h \
    Responses/orderacceptedmsg.h \
    Responses/shownewordermsg.h \
    Responses/listofstocksmsg.h \
    Responses/stockinfomsg.h \
    Responses/listofordersmsg.h \
    Requests/getcompanystatusmsg.h \
    Responses/companystatusmsg.h \
    Responses/orderchangemsg.h \
    Responses/lasttransactionmsg.h \
    Responses/ordercompletedmsg.h \
    Responses/shownobestordermsg.h \
    DataTransferObjects/stockinfo.h \
    DataTransferObjects/userstocks.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path += /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(debug, debug|release) {
    DLLDESTDIR += $$OUT_PWD/../NetworkProtocol_Test/debug/
    message("Copying to NetworkProtocol_Test debug directory.")
} else:win32:CONFIG(debug, debug|release) {
    DLLDESTDIR += $$OUT_PWD/../NetworkProtocol_Test/release/
    message("Copying to NetworkProtocol_Test release directory.")
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Utilities/Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Utilities/Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../../Utilities/Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../../Utilities/Utilities
DEPENDPATH += $$PWD/../../Utilities/Utilities

win32:gcc:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/release/libUtilities.a
else:win32:gcc:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/debug/libUtilities.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/libUtilities.a

