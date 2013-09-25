QMAKE_CXXFLAGS += -std=gnu++11 -Wall -Wextra -Wfloat-equal -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    DataTransferObjects/order.cpp \
    IMessages/unsubscribestockmsg.cpp \
    IMessages/subscribestockmsg.cpp \
    IMessages/sellstockreqmsg.cpp \
    IMessages/registeruserreqmsg.cpp \
    IMessages/loginuserreqmsg.cpp \
    IMessages/imessage.cpp \
    IMessages/getstockinfomsg.cpp \
    IMessages/getmystocksmsg.cpp \
    IMessages/getmyordermsg.cpp \
    IMessages/companystatus.cpp \
    IMessages/cancelordermsg.cpp \
    IMessages/buystockreqmsg.cpp \
    OMessages/unrecognizedusermsg.cpp \
    OMessages/transactionchange.cpp \
    OMessages/selltransactionmsg.cpp \
    OMessages/registeruserrespok.cpp \
    OMessages/registeruserrespfail.cpp \
    OMessages/ordermsg.cpp \
    OMessages/orderidresp.cpp \
    OMessages/omessage.cpp \
    OMessages/loginuserrespok.cpp \
    OMessages/loginuserrespfail.cpp \
    OMessages/getstockinforespmsg.cpp \
    OMessages/getmystocksrespmsg.cpp \
    OMessages/getmyordersrespmsg.cpp \
    OMessages/companyfrozen.cpp \
    OMessages/companyactive.cpp \
    OMessages/buytransactionmsg.cpp \
    OMessages/bestorder.cpp \
    iomessage.cpp

HEADERS +=\
        networkprotocol_global.h \
    DataTransferObjects/order.h \
    IMessages/unsubscribestockmsg.h \
    IMessages/subscribestockmsg.h \
    IMessages/sellstockreqmsg.h \
    IMessages/registeruserreqmsg.h \
    IMessages/loginuserreqmsg.h \
    IMessages/imessage.h \
    IMessages/getstockinfomsg.h \
    IMessages/getmystocksmsg.h \
    IMessages/getmyordermsg.h \
    IMessages/companystatus.h \
    IMessages/cancelordermsg.h \
    IMessages/buystockreqmsg.h \
    OMessages/unrecognizedusermsg.h \
    OMessages/transactionchange.h \
    OMessages/selltransactionmsg.h \
    OMessages/registeruserrespok.h \
    OMessages/registeruserrespfail.h \
    OMessages/ordermsg.h \
    OMessages/orderidresp.h \
    OMessages/omessage.h \
    OMessages/loginuserrespok.h \
    OMessages/loginuserrespfail.h \
    OMessages/getstockinforespmsg.h \
    OMessages/getmystocksrespmsg.h \
    OMessages/getmyordersrespmsg.h \
    OMessages/companyfrozen.h \
    OMessages/companyactive.h \
    OMessages/buytransactionmsg.h \
    OMessages/bestorder.h \
    iomessage.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
