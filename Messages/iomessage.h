#ifndef IOMESSAGE_H
#define IOMESSAGE_H

#include <QtGlobal>

class IOMessage
{  
public:
    enum MessageType : qint8
    {
        REGISTER_USER_REQ,
        REGISTER_USER_RESP_OK,
        REGISTER_USER_RESP_FAIL,
        LOGIN_USER_REQ,
        LOGIN_USER_RESP_OK,
        LOGIN_USER_RESP_FAIL,
        SUBSCRIBE_STOCK,
        UNSUBSCRIBE_STOCK,
        SELL_STOCK_REQ,         //zlecenie sprzedazy
        SELL_STOCK_RESP,         //wiadomosc o zrealizowanej transakcji
        BUY_STOCK_REQ,          //zlecenie kupna
        BUY_STOCK_RESP,         //wiadomosc o zrealizowanej transakcji
        GET_STOCKS,         //poproszenei serwera o wysłąnie listy akcji
        LIST_OF_STOCKS,     //wysyłanie przez serwer listy akcji
        CHANGE_PRICE,        //wysłanie do użytkownika informacji o zmianie cen akcji
        COMPANY_STATUS_REQ,     // IMessage
        COMPANY_ACTIVE_RESP,
        COMPANY_FROZEN_RESP,
        UNDEFINED,

        SESSION_STARTED,
        SESSION_CLOSED,
        IS_SESSION_ACTIVE,
        SESSION_STATUS,

        UNRECOGNIZED_USER   // wysyłanie informacji potrzebie zalogowania by korzystać
                            // z pewnej funkcjonalności
    };

    virtual MessageType type() const = 0;
protected:
    static MessageType toType(qint8);
    IOMessage();
};

#endif // MESSAGE_H
