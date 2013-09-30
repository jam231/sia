QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wfloat-equal -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    ../NetworkProtocol/DataTransferObjects/order.cpp \
    ../NetworkProtocol/Requests/unsubscribestockmsg.cpp \
    ../NetworkProtocol/Requests/subscribestockmsg.cpp \
    ../NetworkProtocol/Requests/sellstockreqmsg.cpp \
    ../NetworkProtocol/Requests/registeruserreqmsg.cpp \
    ../NetworkProtocol/Requests/loginuserreqmsg.cpp \
    ../NetworkProtocol/Requests/getstockinfomsg.cpp \
    ../NetworkProtocol/Requests/getmystocksmsg.cpp \
    ../NetworkProtocol/Requests/getmyordermsg.cpp \
    ../NetworkProtocol/Requests/companystatus.cpp \
    ../NetworkProtocol/Requests/cancelordermsg.cpp \
    ../NetworkProtocol/Requests/buystockreqmsg.cpp \
    ../NetworkProtocol/Responses/unrecognizedusermsg.cpp \
    ../NetworkProtocol/Responses/transactionchange.cpp \
    ../NetworkProtocol/Responses/selltransactionmsg.cpp \
    ../NetworkProtocol/Responses/registeruserrespok.cpp \
    ../NetworkProtocol/Responses/registeruserrespfail.cpp \
    ../NetworkProtocol/Responses/ordermsg.cpp \
    ../NetworkProtocol/Responses/orderidresp.cpp \
    ../NetworkProtocol/Responses/loginuserrespok.cpp \
    ../NetworkProtocol/Responses/loginuserrespfail.cpp \
    ../NetworkProtocol/Responses/getstockinforespmsg.cpp \
    ../NetworkProtocol/Responses/getmystocksrespmsg.cpp \
    ../NetworkProtocol/Responses/getmyordersrespmsg.cpp \
    ../NetworkProtocol/Responses/companyfrozen.cpp \
    ../NetworkProtocol/Responses/companyactive.cpp \
    ../NetworkProtocol/Responses/buytransactionmsg.cpp \
    ../NetworkProtocol/Responses/bestordermsg.cpp \
    ../NetworkProtocol/Requests/request.cpp \
    ../NetworkProtocol/Responses/response.cpp \
    ../NetworkProtocol/message.cpp \
    DataTransferObjects/bestorder.cpp \
    DataTransferObjects/types.cpp \
    DataTransferObjects/lasttransaction.cpp

HEADERS += \
    ../NetworkProtocol/networkprotocol_global.h \
    ../NetworkProtocol/DataTransferObjects/order.h \
    ../NetworkProtocol/Requests/unsubscribestockmsg.h \
    ../NetworkProtocol/Requests/subscribestockmsg.h \
    ../NetworkProtocol/Requests/sellstockreqmsg.h \
    ../NetworkProtocol/Requests/registeruserreqmsg.h \
    ../NetworkProtocol/Requests/loginuserreqmsg.h \
    ../NetworkProtocol/Requests/getstockinfomsg.h \
    ../NetworkProtocol/Requests/getmystocksmsg.h \
    ../NetworkProtocol/Requests/getmyordermsg.h \
    ../NetworkProtocol/Requests/companystatus.h \
    ../NetworkProtocol/Requests/cancelordermsg.h \
    ../NetworkProtocol/Requests/buystockreqmsg.h \
    ../NetworkProtocol/Responses/unrecognizedusermsg.h \
    ../NetworkProtocol/Responses/transactionchange.h \
    ../NetworkProtocol/Responses/selltransactionmsg.h \
    ../NetworkProtocol/Responses/registeruserrespok.h \
    ../NetworkProtocol/Responses/registeruserrespfail.h \
    ../NetworkProtocol/Responses/ordermsg.h \
    ../NetworkProtocol/Responses/orderidresp.h \
    ../NetworkProtocol/Responses/loginuserrespok.h \
    ../NetworkProtocol/Responses/loginuserrespfail.h \
    ../NetworkProtocol/Responses/getstockinforespmsg.h \
    ../NetworkProtocol/Responses/getmystocksrespmsg.h \
    ../NetworkProtocol/Responses/getmyordersrespmsg.h \
    ../NetworkProtocol/Responses/companyfrozen.h \
    ../NetworkProtocol/Responses/companyactive.h \
    ../NetworkProtocol/Responses/buytransactionmsg.h \
    ../NetworkProtocol/Responses/bestordermsg.h \
    ../NetworkProtocol/Requests/request.h \
    ../NetworkProtocol/Responses/response.h \
    ../NetworkProtocol/message.h \
    DataTransferObjects/bestorder.h \
    DataTransferObjects/types.h \
    DataTransferObjects/lasttransaction.h



unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
