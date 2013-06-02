#ifndef IOMESSAGE_H
#define IOMESSAGE_H

#include <QtGlobal>

class IOMessage
{  
public:
    enum MessageType : qint8
    {
        //rejestracja, logowanie
        REGISTER_USER_REQ,
        REGISTER_USER_RESP_OK,
        REGISTER_USER_RESP_FAIL,
        LOGIN_USER_REQ,
        LOGIN_USER_RESP_OK,
        LOGIN_USER_RESP_FAIL,

        SELL_STOCK_REQ,     // IMessage zlecenie kupna
        BUY_STOCK_REQ,      // IMessage zlecenie sprzedazy
        TRANSACTION_CHANGE, // OMessage powiadomienie o zmianie (zrealizowaniu?) danej transakcji wszystki
                            // zasubskrybowanych
        ORDER,          // OMessage informacja o nowym zleceniu
        BEST_ORDER,     // OMessage informacja o nowym najlepszym zleceniu
        SUBSCRIBE_STOCK,    // IMessage subskrybuj
        UNSUBSCRIBE_STOCK,  // IMessage przestan subskrybowac

        GET_MY_STOCKS,      // IMessage prosba o liste wlasnych zasobów
        GET_MY_STOCKS_RESP,  // OMessage Odeslanie listy zasobow
        GET_MY_ORDERS,      // IMessage prosba o liste wlasnych zasobów
        GET_MY_ORDERS_RESP,  // OMessage Odeslanie listy zasobow
        GET_STOCK_INFO,         // IMessage prosba o info o danym zasobie
        GET_STOCK_INFO_RESP,    // OMessage odpowiedz na prosbe o info

// O tych nie wiem co myslec... -jam231

        COMPANY_STATUS_REQ,
        COMPANY_ACTIVE_RESP,
        COMPANY_FROZEN_RESP,
//
// Te sa byc moze tymczasowe
        BUY_TRANSACTION,    // OMessage wysylanie informacji o danej transakcji kupna
        SELL_TRANSACTION,   // OMessage wysylanie informacji o danej transakcji sprzedazy

// Te sa w zawieszeniu, czekaja na implementacje lub zmiane -jam231
        SESSION_STARTED,
        SESSION_CLOSED,
        IS_SESSION_ACTIVE,
        SESSION_STATUS,
//
        UNDEFINED,
        UNRECOGNIZED_USER   // wysyłanie informacji potrzebie zalogowania by korzystać
                            // z pewnej funkcjonalności
    };

    virtual MessageType type() const = 0;
protected:
    static MessageType toType(qint8);
    IOMessage();
public:
    virtual ~IOMessage() {}
};

#endif // MESSAGE_H
