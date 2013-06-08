#ifndef IOMESSAGE_H
#define IOMESSAGE_H

#include <QtGlobal>

class IOMessage
{  
public:
    enum MessageType : qint8
    {

        /*0-19: komunikacja z serwerem, logowanie, etc*/

        REGISTER_USER_REQ = 0x00,
        REGISTER_USER_RESP_OK,
        REGISTER_USER_RESP_FAIL,
        LOGIN_USER_REQ,
        LOGIN_USER_RESP_OK,
        LOGIN_USER_RESP_FAIL,
        UNRECOGNIZED_USER,   // wysyłanie informacji potrzebie zalogowania by korzystać
                            // z pewnej funkcjonalności





        /*20-39: akcje i zlecenia*/

        SELL_STOCK_REQ = 0x14,  // IMessage zlecenie kupna
        BUY_STOCK_REQ,          // IMessage zlecenie sprzedazy
        // Te sa byc moze tymczasowe
        BUY_TRANSACTION,        // OMessage wysylanie informacji o danej transakcji kupna
        SELL_TRANSACTION,       // OMessage wysylanie informacji o danej transakcji sprzedazy


        TRANSACTION_CHANGE,     // OMessage powiadomienie o zmianie (zrealizowaniu?) danej transakcji
                                // wszystkich zasubskrybowanych

        ORDER,                  // OMessage informacja o nowym zleceniu
        BEST_ORDER,             // OMessage informacja o nowym najlepszym zleceniu

        SUBSCRIBE_STOCK,        // IMessage subskrybuj
        UNSUBSCRIBE_STOCK,      // IMessage przestan subskrybowac


        GET_MY_STOCKS,          // IMessage prosba o liste wlasnych zasobów
        GET_MY_STOCKS_RESP,     // OMessage Odeslanie listy zasobow
        GET_MY_ORDERS,          // IMessage prosba o liste wlasnych zasobów
        GET_MY_ORDERS_RESP,     // OMessage Odeslanie listy zasobow
        GET_STOCK_INFO,         // IMessage prosba o info o danym zasobie
        GET_STOCK_INFO_RESP,    // OMessage odpowiedz na prosbe o info

        CANCEL_ORDER_REQ,       // IMessage żadanie anulowania zlecenia o podanym id.
        ORDER_ID_RESP,


        /*40-49: sesja, stan akcji */

        COMPANY_STATUS_REQ = 0x28,
        COMPANY_ACTIVE_RESP,
        COMPANY_FROZEN_RESP,

        // Te sa w zawieszeniu, czekaja na implementacje lub zmiane -jam231
        SESSION_STARTED,
        SESSION_CLOSED,
        IS_SESSION_ACTIVE,
        SESSION_STATUS,



        /*50-99 zostawiam*/



        /*100+ inne*/
        UNDEFINED = 0x64
    };

    virtual MessageType type() const = 0;
protected:
    static MessageType toType(qint8);
    IOMessage();
public:
    virtual ~IOMessage() {}
};

#endif // MESSAGE_H
