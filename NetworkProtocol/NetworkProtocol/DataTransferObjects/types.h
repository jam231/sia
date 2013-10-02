#ifndef TYPES_H
#define TYPES_H

#include "networkprotocol_global.h"

#include <QDataStream>
#include <QDebug>
#include <QHash>

namespace NetworkProtocol
{
namespace DTO
{
namespace Types
{
    template<typename ValueType>
    struct ContainerType
    {
        typedef ValueType UnderlyingType;
        ValueType value;

        ContainerType();
        ContainerType(ValueType);
    };

    template<typename ValueType>
    inline uint qHash(const ContainerType<ValueType>& key, uint seed = 0) Q_DECL_NOTHROW
    {
        return ::qHash(key.value, seed);
    }

    template<typename ValueType>
    QDataStream &operator<<(QDataStream& stream, const ContainerType<ValueType>&);
    template<typename ValueType>
    QDataStream &operator>>(QDataStream& stream, ContainerType<ValueType>&);
    template<typename ValueType>
    QDebug &operator<<(QDebug& stream, const ContainerType<ValueType>&);
    template<typename ValueType>
    QDebug &operator>>(QDebug& stream, ContainerType<ValueType>&);

    /*
     *  Najpiekniej by bylo, gdyby kompilator potrafil sie domyslic
     *  ze _ContainerType posiada UnderlyingType, ale niestety nie tak to dziala
     *                                                  -- jam231
     */
    template<class _ContainerType, typename _ContainerUnderlyingType>
    class ContainerTypeOperations
    {
    private:
        // With _ContainerUnderlyingType as RHS
        _ContainerType operator=(const _ContainerUnderlyingType&);
    public:

        // With _ContainerType as RHS
        //_ContainerType operator=(const _ContainerType&);


        // Arthemtic operators
        _ContainerType& operator-=(const _ContainerType&);
        _ContainerType& operator+=(const _ContainerType&);

        // Comparison operators
        bool operator==(const _ContainerType&) const;
        bool operator<=(const _ContainerType&) const;
        bool operator>=(const _ContainerType&) const;
        bool operator!=(const _ContainerType&) const;
        bool operator<(const _ContainerType&) const;
        bool operator>(const _ContainerType&) const;



        // Arthemtic operators
        _ContainerType& operator-=(const _ContainerUnderlyingType&);
        _ContainerType& operator+=(const _ContainerUnderlyingType&);

        // Comparison operators
        bool operator==(const _ContainerUnderlyingType) const;
        bool operator<=(const _ContainerUnderlyingType) const;
        bool operator>=(const _ContainerUnderlyingType) const;
        bool operator!=(const _ContainerUnderlyingType) const;
        bool operator<(const _ContainerUnderlyingType) const;
        bool operator>(const _ContainerUnderlyingType) const;

    };


    class NETWORKPROTOCOLSHARED_EXPORT OrderIdType :    public ContainerType<qint32>,
                                                         public ContainerTypeOperations<OrderIdType, qint32>
    {
    public:
        OrderIdType();
        OrderIdType(qint32);
    };

    class NETWORKPROTOCOLSHARED_EXPORT StockIdType :    public ContainerType<qint32>,
                                                         public ContainerTypeOperations<StockIdType, qint32>
    {
    public:
        StockIdType();
        StockIdType(qint32);
    };

    class NETWORKPROTOCOLSHARED_EXPORT UserIdType :     public ContainerType<qint32>,
                                                         public ContainerTypeOperations<UserIdType, qint32>
    {
    public:
        UserIdType();
        UserIdType(qint32);
    };

    class NETWORKPROTOCOLSHARED_EXPORT CompanyIdType :  public ContainerType<qint32>,
                                                        public ContainerTypeOperations<CompanyIdType, qint32>
    {
    public:
        CompanyIdType();
        CompanyIdType(qint32);
    };

    class NETWORKPROTOCOLSHARED_EXPORT AmountType :     public ContainerType<qint32>,
                                                         public ContainerTypeOperations<AmountType, qint32>
    {
    public:
        AmountType();
        AmountType(qint32);
    };

    class NETWORKPROTOCOLSHARED_EXPORT PriceType :      public ContainerType<qint32>,
                                                        public ContainerTypeOperations<PriceType, qint32>
    {
    public:
        PriceType();
        PriceType(qint32);
    };

    // Ids
    //typedef qint32 OrderIdType;
    //typedef qint32 StockIdType;
    //typedef qint32 UserIdType;
    //typedef qint32 CompanyIdType;
    //typedef qint32 AmountType;
    //typedef qint32 PriceType;

    typedef qint16 MessageLengthType;


    // Force separation
    namespace Order
    {
    typedef qint8 OrderTypeType;
    enum OrderType : OrderTypeType
    {
        UNDEFINED = 0,
        BUY,
        SELL
    };

    NETWORKPROTOCOLSHARED_EXPORT OrderType toOrderType(OrderTypeType);
    }


    // Force separation
    namespace Message
    {
    typedef qint8 MessageTypeType;
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

    NETWORKPROTOCOLSHARED_EXPORT MessageType toMessageType(MessageTypeType);
    }

    //                                                      TEMPLATE DEFINITIONS



    //template<typename ValueType>
    //class ContainerType

    template<typename ValueType>
    ContainerType<ValueType>::ContainerType() : value(ValueType())
    {
    }
    template<typename ValueType>
    ContainerType<ValueType>::ContainerType(ValueType _value) : value(_value)
    {
    }

    template<typename ValueType>
    QDataStream &operator<<(QDataStream& stream, const ContainerType<ValueType>& containerType)
    {
        stream << containerType.value;
        return stream;
    }

    template<typename ValueType>
    QDataStream &operator>>(QDataStream& stream, ContainerType<ValueType>& containerType)
    {
        stream >> containerType.value;
        return stream;
    }

    template<typename ValueType>
    QDebug &operator<<(QDebug& stream, const ContainerType<ValueType>& containerType)
    {
        stream << containerType.value;
        return stream;
    }

    template<typename ValueType>
    QDebug &operator>>(QDebug& stream, ContainerType<ValueType>& containerType)
    {
        stream >> containerType.value;
        return stream;
    }

    //template<class _ContainerType>
    //class ContainerTypeOperations

    // With _ContainerType as RHS
/*
    template<class _ContainerType, typename _ContainerUnderlyingType>
    _ContainerType ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator=(const _ContainerType& rhs)
    {
        return _ContainerType(rhs.value);
    }
*/
        // Arthemtic operators
    template<class _ContainerType, typename _ContainerUnderlyingType>
    _ContainerType& ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator-=(const _ContainerType& rhs)
    {
        this->value -= rhs.value;
        return this;
    }

    template<class _ContainerType, typename _ContainerUnderlyingType>
    _ContainerType& ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator+=(const _ContainerType& rhs)
    {
        this->value += rhs.value;
        return this;
    }

        // Comparison operators
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator==(const _ContainerType& rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value == rhs.value;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator>=(const _ContainerType& rhs) const
    {
        return this->value >= rhs.value;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator<=(const _ContainerType& rhs) const
    {
        return this->value <= rhs.value;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator!=(const _ContainerType& rhs) const
    {
        return this->value != rhs.value;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator>(const _ContainerType& rhs) const
    {
        return this->value > rhs.value;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator<(const _ContainerType& rhs) const
    {
        return this->value < rhs.value;
    }

    //         With _ContainerUnderlyingType as RHS
/*
    template<class _ContainerType, typename _ContainerUnderlyingType>
    _ContainerType ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator=(const _ContainerUnderlyingType& rhs)
    {
        return _ContainerType(rhs);
    }
*/
        // Arthemtic operators
    template<class _ContainerType, typename _ContainerUnderlyingType>
    _ContainerType& ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator-=(const _ContainerUnderlyingType& rhs)
    {
        this->value -= rhs;
        return this;
    }

    template<class _ContainerType, typename _ContainerUnderlyingType>
    _ContainerType& ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator+=(const _ContainerUnderlyingType& rhs)
    {
        this->value += rhs;
        return this;
    }

        // Comparison operators
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator==(const _ContainerUnderlyingType rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value == rhs;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator>=(_ContainerUnderlyingType rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value >= rhs;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator<=(_ContainerUnderlyingType rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value <= rhs;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator!=(const _ContainerUnderlyingType rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value != rhs;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator>(const _ContainerUnderlyingType rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value > rhs;
    }
    template<class _ContainerType, typename _ContainerUnderlyingType>
    bool ContainerTypeOperations<_ContainerType, _ContainerUnderlyingType>::operator<(const _ContainerUnderlyingType rhs) const
    {
        return static_cast<const _ContainerType*>(this)->value < rhs;
    }

}

// Outside of Types namespace
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator>>(QDataStream& stream, Types::Message::MessageType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator>>(QDataStream& stream, Types::Order::OrderType&);

}
}
#endif // TYPES_H
