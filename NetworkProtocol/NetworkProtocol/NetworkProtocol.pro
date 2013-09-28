QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wfloat-equal -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    ../NetworkProtocol/DataTransferObjects/order.cpp \
    ../NetworkProtocol/IMessages/unsubscribestockmsg.cpp \
    ../NetworkProtocol/IMessages/subscribestockmsg.cpp \
    ../NetworkProtocol/IMessages/sellstockreqmsg.cpp \
    ../NetworkProtocol/IMessages/registeruserreqmsg.cpp \
    ../NetworkProtocol/IMessages/loginuserreqmsg.cpp \
    ../NetworkProtocol/IMessages/imessage.cpp \
    ../NetworkProtocol/IMessages/getstockinfomsg.cpp \
    ../NetworkProtocol/IMessages/getmystocksmsg.cpp \
    ../NetworkProtocol/IMessages/getmyordermsg.cpp \
    ../NetworkProtocol/IMessages/companystatus.cpp \
    ../NetworkProtocol/IMessages/cancelordermsg.cpp \
    ../NetworkProtocol/IMessages/buystockreqmsg.cpp \
    ../NetworkProtocol/OMessages/unrecognizedusermsg.cpp \
    ../NetworkProtocol/OMessages/transactionchange.cpp \
    ../NetworkProtocol/OMessages/selltransactionmsg.cpp \
    ../NetworkProtocol/OMessages/registeruserrespok.cpp \
    ../NetworkProtocol/OMessages/registeruserrespfail.cpp \
    ../NetworkProtocol/OMessages/ordermsg.cpp \
    ../NetworkProtocol/OMessages/orderidresp.cpp \
    ../NetworkProtocol/OMessages/omessage.cpp \
    ../NetworkProtocol/OMessages/loginuserrespok.cpp \
    ../NetworkProtocol/OMessages/loginuserrespfail.cpp \
    ../NetworkProtocol/OMessages/getstockinforespmsg.cpp \
    ../NetworkProtocol/OMessages/getmystocksrespmsg.cpp \
    ../NetworkProtocol/OMessages/getmyordersrespmsg.cpp \
    ../NetworkProtocol/OMessages/companyfrozen.cpp \
    ../NetworkProtocol/OMessages/companyactive.cpp \
    ../NetworkProtocol/OMessages/buytransactionmsg.cpp \
    ../NetworkProtocol/OMessages/bestorder.cpp \
    ../NetworkProtocol/iomessage.cpp

HEADERS += \
    ../NetworkProtocol/networkprotocol_global.h \
    ../NetworkProtocol/DataTransferObjects/order.h \
    ../NetworkProtocol/IMessages/unsubscribestockmsg.h \
    ../NetworkProtocol/IMessages/subscribestockmsg.h \
    ../NetworkProtocol/IMessages/sellstockreqmsg.h \
    ../NetworkProtocol/IMessages/registeruserreqmsg.h \
    ../NetworkProtocol/IMessages/loginuserreqmsg.h \
    ../NetworkProtocol/IMessages/imessage.h \
    ../NetworkProtocol/IMessages/getstockinfomsg.h \
    ../NetworkProtocol/IMessages/getmystocksmsg.h \
    ../NetworkProtocol/IMessages/getmyordermsg.h \
    ../NetworkProtocol/IMessages/companystatus.h \
    ../NetworkProtocol/IMessages/cancelordermsg.h \
    ../NetworkProtocol/IMessages/buystockreqmsg.h \
    ../NetworkProtocol/OMessages/unrecognizedusermsg.h \
    ../NetworkProtocol/OMessages/transactionchange.h \
    ../NetworkProtocol/OMessages/selltransactionmsg.h \
    ../NetworkProtocol/OMessages/registeruserrespok.h \
    ../NetworkProtocol/OMessages/registeruserrespfail.h \
    ../NetworkProtocol/OMessages/ordermsg.h \
    ../NetworkProtocol/OMessages/orderidresp.h \
    ../NetworkProtocol/OMessages/omessage.h \
    ../NetworkProtocol/OMessages/loginuserrespok.h \
    ../NetworkProtocol/OMessages/loginuserrespfail.h \
    ../NetworkProtocol/OMessages/getstockinforespmsg.h \
    ../NetworkProtocol/OMessages/getmystocksrespmsg.h \
    ../NetworkProtocol/OMessages/getmyordersrespmsg.h \
    ../NetworkProtocol/OMessages/companyfrozen.h \
    ../NetworkProtocol/OMessages/companyactive.h \
    ../NetworkProtocol/OMessages/buytransactionmsg.h \
    ../NetworkProtocol/OMessages/bestorder.h \
    ../NetworkProtocol/iomessage.h



unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
