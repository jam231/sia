#ifndef IOMESSAGE_H
#define IOMESSAGE_H

#include <QtGlobal>

class IOMessage
{  
public:
    enum MessageType : qint8
    {
        REGISTER_USER_REQ,
        REGISTER_USER_RESP,
        LOGIN_USER,
        SUBSCRIBE_STOCK,
        UNSUBSCRIBE_STOCK,
        SELL_STOCK_REQ,         //zlecenie sprzedarzy
        SELL_STOCK_RESP,         //wiadomosc o zrealizowanej transakcji
        BUY_STOCK_REQ,          //zlecenie kupna
        BUY_STOCK_RESP,         //wiadomosc o zrealizowanej transakcji
        GET_STOCKS,         //poproszenei serwera o wysłąnie listy akcji
        LIST_OF_STOCKS,     //wysąłnie przez serwer listy akcji
        CHANGE_PRICE,        //wysłanie do użytkownika informacji o zmianie cen akcji
        UNRECOGNIZED_USER,   //odpowiedz serwera na wiadomosc uzytkownika ktory sie wczesniej nie zalogowal
        UNDEFINED
    };

    virtual MessageType type() const = 0;
protected:
    static MessageType toType(qint8);
    IOMessage();
};

#endif // MESSAGE_H
