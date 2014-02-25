QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    ../NetworkProtocol/Requests/unsubscribestockmsg.cpp \
    ../NetworkProtocol/Requests/subscribestockmsg.cpp \
    ../NetworkProtocol/Requests/sellstockmsg.cpp \
    ../NetworkProtocol/Requests/registerusermsg.cpp \
    ../NetworkProtocol/Requests/loginusermsg.cpp \
    ../NetworkProtocol/Requests/getstockinfomsg.cpp \
    ../NetworkProtocol/Requests/getmystocksmsg.cpp \
    ../NetworkProtocol/Requests/getmyordersmsg.cpp \
    ../NetworkProtocol/Requests/companystatusmsg.cpp \
    ../NetworkProtocol/Requests/cancelordermsg.cpp \
    ../NetworkProtocol/Requests/buystockmsg.cpp \
    ../NetworkProtocol/Responses/transactionchangemsg.cpp \
    ../NetworkProtocol/Responses/selltransactionmsg.cpp \
    ../NetworkProtocol/Responses/showordermsg.cpp \
    ../NetworkProtocol/Responses/orderidmsg.cpp \
    ../NetworkProtocol/Responses/okmsg.cpp \
    ../NetworkProtocol/Responses/failuremsg.cpp \
    ../NetworkProtocol/Responses/companyfrozenmsg.cpp \
    ../NetworkProtocol/Responses/companyactivemsg.cpp \
    ../NetworkProtocol/Responses/buytransactionmsg.cpp \
    ../NetworkProtocol/Responses/showbestordermsg.cpp \
    ../NetworkProtocol/Responses/stockinfomsg.cpp \
    ../NetworkProtocol/Responses/showuserordersmsg.cpp \
    ../NetworkProtocol/Responses/showuserstocksmsg.cpp \
    ../NetworkProtocol/Requests/request.cpp \
    ../NetworkProtocol/Responses/response.cpp \
    DataTransferObjects/order.cpp \
    DataTransferObjects/bestorder.cpp \
    DataTransferObjects/types.cpp \
    DataTransferObjects/lasttransaction.cpp \
    Responses/registerusersuccessmsg.cpp


HEADERS += \
    ../NetworkProtocol/networkprotocol_global.h \
    ../NetworkProtocol/Requests/unsubscribestockmsg.h \
    ../NetworkProtocol/Requests/subscribestockmsg.h \
    ../NetworkProtocol/Requests/sellstockmsg.h \
    ../NetworkProtocol/Requests/registerusermsg.h \
    ../NetworkProtocol/Requests/loginusermsg.h \
    ../NetworkProtocol/Requests/getstockinfomsg.h \
    ../NetworkProtocol/Requests/getmystocksmsg.h \
    ../NetworkProtocol/Requests/getmyordersmsg.h \
    ../NetworkProtocol/Requests/companystatusmsg.h \
    ../NetworkProtocol/Requests/cancelordermsg.h \
    ../NetworkProtocol/Requests/buystockmsg.h \
    ../NetworkProtocol/Responses/transactionchangemsg.h \
    ../NetworkProtocol/Responses/selltransactionmsg.h \
    ../NetworkProtocol/Responses/showordermsg.h \
    ../NetworkProtocol/Responses/orderidmsg.h \
    ../NetworkProtocol/Responses/okmsg.h \
    ../NetworkProtocol/Responses/failuremsg.h \
    ../NetworkProtocol/Responses/companyfrozenmsg.h \
    ../NetworkProtocol/Responses/companyactivemsg.h \
    ../NetworkProtocol/Responses/buytransactionmsg.h \
    ../NetworkProtocol/Responses/showbestordermsg.h \
    ../NetworkProtocol/Responses/stockinfomsg.h \
    ../NetworkProtocol/Responses/showuserordersmsg.h \
    ../NetworkProtocol/Responses/showuserstocksmsg.h \
    ../NetworkProtocol/Requests/request.h \
    ../NetworkProtocol/Responses/response.h \
    ../NetworkProtocol/message.h \
    DataTransferObjects/order.h \
    DataTransferObjects/bestorder.h \
    DataTransferObjects/types.h \
    DataTransferObjects/lasttransaction.h \
    Responses/registerusersuccessmsg.h




unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


win32:gcc:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Utilities/Utilities/release/ -lUtilities
else:win32:gcc:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Utilities/Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../../Utilities/Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../../Utilities/Utilities
DEPENDPATH += $$PWD/../../Utilities/Utilities

win32:gcc:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/release/libUtilities.a
else:win32:gcc:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/debug/libUtilities.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/libUtilities.a
