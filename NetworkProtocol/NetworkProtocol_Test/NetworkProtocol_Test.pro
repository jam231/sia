QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra  -Wfloat-equal -Wundef \
                  -Wpointer-arith

TARGET = NetworkProtocol_TEST

QT       += network testlib

QT       -= gui

CONFIG += console
CONFIG += testcase
CONFIG -= app_bundle

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    main.cpp \
    message_test.cpp \
    DataTransferObjects/types_test.cpp \
    DataTransferObjects/order_test.cpp \
    DataTransferObjects/lasttransaction_test.cpp \
    DataTransferObjects/bestorder_test.cpp \
    Requests/unsubscribestockmsg_test.cpp \
    Requests/subscribestockmsg_test.cpp \
    Requests/sellstockmsg_test.cpp \
    Requests/request_test.cpp \
    Requests/registerusermsg_test.cpp \
    Requests/loginusermsg_test.cpp \
    Requests/getstockinfomsg_test.cpp \
    Requests/getmystocksmsg_test.cpp \
    Requests/getmyordersmsg_test.cpp \
    Requests/companystatusmsg_test.cpp \
    Requests/cancelordermsg_test.cpp \
    Requests/buystockmsg_test.cpp \
    Responses/unrecognizedusermsg_test.cpp \
    Responses/transactionchangemsg_test.cpp \
    Responses/stockinfomsg_test.cpp \
    Responses/showuserordersmsg_test.cpp \
    Responses/showordermsg_test.cpp \
    Responses/selltransactionmsg_test.cpp \
    Responses/response_test.cpp \
    Responses/registerusersuccessmsg_test.cpp \
    Responses/registeruserfailuremsg_test.cpp \
    Responses/orderidmsg_test.cpp \
    Responses/loginusersuccessmsg_test.cpp \
    Responses/loginuserfailuremsg_test.cpp \
    Responses/howuserstocksmsg_test.cpp \
    Responses/companyfrozenmsg_test.cpp \
    Responses/companyactivemsg_test.cpp \
    Responses/buytransactionmsg_test.cpp \
    Responses/bestordermsg_test.cpp

HEADERS += \
    message_test.h \
    DataTransferObjects/types_test.h \
    DataTransferObjects/order_test.h \
    DataTransferObjects/lasttransaction_test.h \
    DataTransferObjects/bestorder_test.h \
    Requests/unsubscribestockmsg_test.h \
    Requests/subscribestockmsg_test.h \
    Requests/sellstockmsg_test.h \
    Requests/request_test.h \
    Requests/registerusermsg_test.h \
    Requests/loginusermsg_test.h \
    Requests/getstockinfomsg_test.h \
    Requests/getmystocksmsg_test.h \
    Requests/getmyordersmsg_test.h \
    Requests/companystatusmsg_test.h \
    Requests/cancelordermsg_test.h \
    Requests/buystockmsg_test.h \
    Responses/unrecognizedusermsg_test.h \
    Responses/transactionchangemsg_test.h \
    Responses/stockinfomsg_test.h \
    Responses/showuserstocksmsg_test.h \
    Responses/showuserordersmsg_test.h \
    Responses/showordermsg_test.h \
    Responses/selltransactionmsg_test.h \
    Responses/response_test.h \
    Responses/registerusersuccessmsg_test.h \
    Responses/registeruserfailuremsg_test.h \
    Responses/orderidmsg_test.h \
    Responses/loginusersuccessmsg_test.h \
    Responses/loginuserfailuremsg_test.h \
    Responses/companyfrozenmsg_test.h \
    Responses/companyactivemsg_test.h \
    Responses/buytransactionmsg_test.h \
    Responses/bestordermsg_test.h

INCLUDEPATH += \
 ../NetworkProtocol/


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/release/ -lNetworkProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetworkProtocol/debug/ -lNetworkProtocol
else:unix: LIBS += -L$$OUT_PWD/../NetworkProtocol/ -lNetworkProtocol

INCLUDEPATH += $$PWD/../NetworkProtocol
DEPENDPATH += $$PWD/../NetworkProtocol
