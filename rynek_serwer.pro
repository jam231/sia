QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wfloat-equal -Wundef \
                  -Wpointer-arith

QT += sql \
    network \
    widgets

HEADERS += \
    server.h \
    market.h \
    Messages/iomessage.h \
    Messages/IMessages/imessage.h \
    Messages/OMessages/omessage.h \
    connection.h \
    Messages/IMessages/registeruserreqmsg.h \
    Messages/IMessages/subscribestockmsg.h \
    Messages/IMessages/unsubscribestockmsg.h \
    Messages/IMessages/sellstockreqmsg.h \
    Messages/IMessages/buystockreqmsg.h \
    Messages/OMessages/changepricemsg.h \
    configmanager.h \
    utilities.h \
    Messages/IMessages/loginuserreqmsg.h \
    Messages/OMessages/loginuserrespok.h \
    Messages/OMessages/loginuserrespfail.h \
    Messages/OMessages/registeruserrespfail.h \
    Messages/OMessages/registeruserrespok.h \
    Messages/OMessages/unrecognizedusermsg.h \
    Messages/IMessages/companystatus.h \
    Messages/OMessages/companyfrozen.h \
    Messages/OMessages/companyactive.h \
    Messages/OMessages/selltransactionmsg.h \
    Messages/OMessages/transactionchange.h \
    Messages/OMessages/bestorder.h \
    Messages/IMessages/getmystocksmsg.h \
    Messages/OMessages/getmystocksrespmsg.h \
    Messages/OMessages/getmyordersrespmsg.h \
    Messages/OMessages/ordermsg.h \
    Messages/IMessages/getmyordermsg.h \
    Messages/IMessages/getstockinfomsg.h \
    Messages/OMessages/getstockinforespmsg.h \
    order.h \
    Messages/IMessages/cancelordermsg.h

SOURCES += \
    server.cpp \
    market.cpp \
    Messages/iomessage.cpp \
    Messages/IMessages/imessage.cpp \
    Messages/OMessages/omessage.cpp \
    connection.cpp \
    Messages/IMessages/registeruserreqmsg.cpp \
    Messages/IMessages/subscribestockmsg.cpp \
    Messages/IMessages/unsubscribestockmsg.cpp \
    Messages/IMessages/sellstockreqmsg.cpp \
    Messages/IMessages/buystockreqmsg.cpp \
    main.cpp \
    configmanager.cpp \
    utilities.cpp \
    Messages/IMessages/loginuserreqmsg.cpp \
    Messages/OMessages/loginuserrespok.cpp \
    Messages/OMessages/loginuserrespfail.cpp \
    Messages/OMessages/registeruserrespfail.cpp \
    Messages/OMessages/registeruserrespok.cpp \
    Messages/OMessages/unrecognizedusermsg.cpp \
    Messages/IMessages/companystatus.cpp \
    Messages/OMessages/companyfrozen.cpp \
    Messages/OMessages/companyactive.cpp \
    Messages/OMessages/buytransactionmsg.cpp \
    Messages/OMessages/selltransactionmsg.cpp \
    Messages/OMessages/transactionchange.cpp \
    Messages/OMessages/bestorder.cpp \
    Messages/OMessages/getmystocksrespmsg.cpp \
    Messages/OMessages/getmyordersrespmsg.cpp \
    Messages/OMessages/ordermsg.cpp \
    Messages/IMessages/getmyordermsg.cpp \
    Messages/IMessages/getmystocksmsg.cpp \
    Messages/IMessages/getstockinfomsg.cpp \
    Messages/OMessages/getstockinforespmsg.cpp \
    order.cpp \
    Messages/IMessages/cancelordermsg.cpp

INCLUDEPATH += Messages \
    Messages/IMessages \
    Messages/OMessages

OTHER_FILES += \
    TODO.txt \
    Files/config.ini






























































