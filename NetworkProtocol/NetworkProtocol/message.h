#ifndef MESSAGE_H
#define MESSAGE_H

#include "networkprotocol_global.h"
#include "DataTransferObjects/types.h"
#include <QString>

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT Message
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
        UNRECOGNIZED_USER, // Odpowiedz dla uzytkownika, gdy probuje
                           // niezalogowany wykonac akcje wymagajaca zalogowania.


        /*20-39: akcje i zlecenia*/

        SELL_STOCK_REQ = 0x14,  // Request zlecenie kupna
        BUY_STOCK_REQ,          // Request zlecenie sprzedazy
        // Te sa byc moze tymczasowe
        BUY_TRANSACTION,        // Response wysylanie informacji o danej transakcji kupna
        SELL_TRANSACTION,       // Response wysylanie informacji o danej transakcji sprzedazy


        TRANSACTION_CHANGE,     // Response powiadomienie o zmianie (zrealizowaniu?) danej transakcji
        // wszystkich zasubskrybowanych

        ORDER,                  // Response informacja o nowym zleceniu
        BEST_ORDER,             // Response informacja o nowym najlepszym zleceniu

        SUBSCRIBE_STOCK,        // Request subskrybuj
        UNSUBSCRIBE_STOCK,      // Request przestan subskrybowac


        GET_MY_STOCKS,          // Request prosba o liste wlasnych zasobów
        GET_MY_STOCKS_RESP,     // Response Odeslanie listy zasobow
        GET_MY_ORDERS,          // Request prosba o liste wlasnych zasobów
        GET_MY_ORDERS_RESP,     // Response Odeslanie listy zasobow
        GET_STOCK_INFO,         // Request prosba o info o danym zasobie
        GET_STOCK_INFO_RESP,    // Response odpowiedz na prosbe o info

        CANCEL_ORDER_REQ,       // Request żadanie anulowania zlecenia o podanym id.
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
protected:
    static MessageType toMessageType(DTO::Types::MessageTypeType);
public:
    // Length of the message in BYTES
    virtual DTO::Types::MessageLengthType length() const = 0;
    virtual MessageType type() const = 0;

    static QString getMessageName();

    virtual ~Message() {}
};
}
#endif // MESSAGE_H
