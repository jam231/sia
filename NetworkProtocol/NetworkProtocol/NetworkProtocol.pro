QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    Requests/unsubscribestockmsg.cpp \
    Requests/subscribestockmsg.cpp \
    Requests/sellstockmsg.cpp \
    Requests/registerusermsg.cpp \
    Requests/loginusermsg.cpp \
    Requests/getstockinfomsg.cpp \
    Requests/getmystocksmsg.cpp \
    Requests/getmyordersmsg.cpp \
    Requests/companystatusmsg.cpp \
    Requests/cancelordermsg.cpp \
    Requests/buystockmsg.cpp \
    Responses/transactionchangemsg.cpp \
    Responses/selltransactionmsg.cpp \
    Responses/showordermsg.cpp \
    Responses/orderidmsg.cpp \
    Responses/okmsg.cpp \
    Responses/failuremsg.cpp \
    Responses/buytransactionmsg.cpp \
    Responses/showbestordermsg.cpp \
    Responses/stockinfomsg.cpp \
    Responses/showuserordersmsg.cpp \
    Responses/showuserstocksmsg.cpp \
    Requests/request.cpp \
    Responses/response.cpp \
    DataTransferObjects/order.cpp \
    DataTransferObjects/bestorder.cpp \
    DataTransferObjects/types.cpp \
    DataTransferObjects/lasttransaction.cpp \
    Responses/registerusersuccessmsg.cpp \
    Responses/companystatusmsg.cpp


HEADERS += \
    networkprotocol_global.h \
    Requests/unsubscribestockmsg.h \
    Requests/subscribestockmsg.h \
    Requests/sellstockmsg.h \
    Requests/registerusermsg.h \
    Requests/loginusermsg.h \
    Requests/getstockinfomsg.h \
    Requests/getmystocksmsg.h \
    Requests/getmyordersmsg.h \
    Requests/companystatusmsg.h \
    Requests/cancelordermsg.h \
    Requests/buystockmsg.h \
    Responses/transactionchangemsg.h \
    Responses/selltransactionmsg.h \
    Responses/showordermsg.h \
    Responses/orderidmsg.h \
    Responses/okmsg.h \
    Responses/failuremsg.h \
    Responses/buytransactionmsg.h \
    Responses/showbestordermsg.h \
    Responses/stockinfomsg.h \
    Responses/showuserordersmsg.h \
    Responses/showuserstocksmsg.h \
    Requests/request.h \
    Responses/response.h \
    message.h \
    DataTransferObjects/order.h \
    DataTransferObjects/bestorder.h \
    DataTransferObjects/types.h \
    DataTransferObjects/lasttransaction.h \
    Responses/registerusersuccessmsg.h \
    Responses/companystatusmsg.h




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
