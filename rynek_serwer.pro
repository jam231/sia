HEADERS += \
    user.h \
    server.h \
    stock.h \
    market.h \
    offer.h \
    transaction.h \
    Messages/iomessage.h \
    Messages/IMessages/imessage.h \
    Messages/OMessages/omessage.h \
    connection.h \
    Messages/IMessages/loginusermsg.h \
    Messages/OMessages/unrecognizedusermsg.h \
    Messages/IMessages/registeruserreqmsg.h \
    Messages/OMessages/registeruserrespmsg.h \
    Messages/IMessages/subscribestockmsg.h \
    Messages/IMessages/unsubscribestockmsg.h \
    Messages/IMessages/sellstockreqmsg.h \
    Messages/IMessages/buystockreqmsg.h \
    Messages/OMessages/sellstockrespmsg.h \
    Messages/OMessages/buystockrespmsg.h \
    Messages/IMessages/getstocksmsg.h \
    Messages/OMessages/listofstocksmsg.h \
    Messages/OMessages/changepricemsg.h

SOURCES += \
    user.cpp \
    server.cpp \
    stock.cpp \
    market.cpp \
    offer.cpp \
    transaction.cpp \
    Messages/iomessage.cpp \
    Messages/IMessages/imessage.cpp \
    Messages/OMessages/omessage.cpp \
    connection.cpp \
    Messages/IMessages/loginusermsg.cpp \
    Messages/OMessages/unrecognizedusermsg.cpp \
    Messages/IMessages/registeruserreqmsg.cpp \
    Messages/OMessages/registeruserrespmsg.cpp \
    Messages/IMessages/subscribestockmsg.cpp \
    Messages/IMessages/unsubscribestockmsg.cpp \
    Messages/IMessages/sellstockreqmsg.cpp \
    Messages/IMessages/buystockreqmsg.cpp \
    Messages/OMessages/sellstockrespmsg.cpp \
    Messages/OMessages/buystockrespmsg.cpp \
    Messages/IMessages/getstocksmsg.cpp \
    Messages/OMessages/listofstocksmsg.cpp \
    Messages/OMessages/changepricemsg.cpp \
    main.cpp

INCLUDEPATH += Messages \
    Messages/IMessages \
    Messages/OMessages

QT += sql \
    network





























































