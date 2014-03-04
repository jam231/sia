QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -Wundef \
                  -Wpointer-arith

QT       += network

QT       -= gui

TARGET = NetworkProtocol
TEMPLATE = lib

DEFINES += NETWORKPROTOCOL_LIBRARY

SOURCES += \
    Responses/transactionchangemsg.cpp \
    Responses/stockinfomsg.cpp \
    Responses/showuserstocksmsg.cpp \
    Responses/showuserordersmsg.cpp \
    Responses/showordermsg.cpp \
    Responses/showbestordermsg.cpp \
    Responses/selltransactionmsg.cpp \
    Responses/response.cpp \
    Responses/registerusersuccessmsg.cpp \
    Responses/orderidmsg.cpp \
    Responses/okmsg.cpp \
    Responses/failuremsg.cpp \
    Responses/buytransactionmsg.cpp \
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
    Requests/companystatusmsg.cpp \
    Requests/cancelordermsg.cpp \
    Requests/buystockmsg.cpp \
    Responses/showcompanystatusmsg.cpp \
    networkprotocol_utilities.cpp




HEADERS += \
    networkprotocol_global.h \
    message.h \
    Responses/transactionchangemsg.h \
    Responses/stockinfomsg.h \
    Responses/showuserstocksmsg.h \
    Responses/showuserordersmsg.h \
    Responses/showordermsg.h \
    Responses/showbestordermsg.h \
    Responses/selltransactionmsg.h \
    Responses/response.h \
    Responses/registerusersuccessmsg.h \
    Responses/orderidmsg.h \
    Responses/okmsg.h \
    Responses/failuremsg.h \
    Responses/buytransactionmsg.h \
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
    Requests/companystatusmsg.h \
    Requests/cancelordermsg.h \
    Requests/buystockmsg.h \
    Responses/showcompanystatusmsg.h \
    networkprotocol_utilities.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


win32:CONFIG(debug, debug|release) {
    DLLDESTDIR += $$OUT_PWD/../NetworkProtocol_TEST/debug/
    message("Copying to Jane Debug Directory.")
}
else:unix:CONFIG(debug, debug|release) {
    DLLDESTDIR += $$OUT_PWD/../NetworkProtocol_TEST
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Utilities/Utilities/release/ -lUtilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Utilities/Utilities/debug/ -lUtilities
else:unix: LIBS += -L$$OUT_PWD/../../Utilities/Utilities/ -lUtilities

INCLUDEPATH += $$PWD/../../Utilities/Utilities
DEPENDPATH += $$PWD/../../Utilities/Utilities

win32:gcc:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/release/libUtilities.a
else:win32:gcc:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/debug/libUtilities.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../Utilities/Utilities/libUtilities.a
