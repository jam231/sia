#ifndef TYPES_H
#define TYPES_H

#include "networkprotocol_global.h"

namespace NetworkProtocol
{
namespace DTO
{
namespace Types
{
    // Ids
    typedef qint32 OrderIdType;
    typedef qint32 StockIdType;
    typedef qint32 UserIdType;
    typedef qint32 CompanyIdType;

    typedef qint32 AmountType;
    typedef qint32 PriceType;

    typedef qint16 MessageLengthType;

    /*
     * Niestety kiedy chcemy mieć typy ładnie zdefiniowane w jednymi miejscu
     * a jednoczesnie chcemy serializowac enumy, musimy zrobic coś takiego - tzn. nic
     * lepszego nie przychodzi mi do glowy.
     *                              -- jam231
     */
    // Enum types
    typedef qint8 MessageTypeType;
    typedef qint8 OrderTypeType;



    enum OrderType : OrderTypeType
    {
        UNDEFINED = 0,
        BUY,
        SELL
    };
    enum MessageType : MessageTypeType
    {
        /*0-19: komunikacja z serwerem, logowanie, etc*/

        REQUEST_REGISTER_USER = 0x00,
        RESPONSE_REGISTER_USER_OK,
        RESPONSE_REGISTER_USER_FAIL,
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
        MESSAGE_UNDEFINED = 0x64
    };

    MessageType toMessageType(MessageTypeType);
    OrderType toOrderType(OrderTypeType);
}
}
}
#endif // TYPES_H
