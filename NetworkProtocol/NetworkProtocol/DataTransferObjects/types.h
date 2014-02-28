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

enum class BasicTypeName
{
    OrderId,
    StockId,
    CompanyId,
    Amount,
    Price,
    UserId
};

template<BasicTypeName TypeName, typename ValueType = qint32, ValueType defaultValue = ValueType{}>
struct ContainerType

{
    ValueType value;
    typedef ValueType UnderlyingType;

    ContainerType(ValueType _value = defaultValue) : value(_value){}

    ContainerType(ContainerType const&) = default;
    ContainerType(ContainerType &&) = default;

    ContainerType& operator=(ContainerType const&) = default;
    ContainerType& operator=(ContainerType &&) = default;

    //operator ValueType() const { return value; }

    ContainerType& operator-=(const ContainerType&);
    ContainerType& operator+=(const ContainerType&);

    // Comparison operators
    bool operator==(const ContainerType&) const;
    bool operator<=(const ContainerType&) const;
    bool operator>=(const ContainerType&) const;
    bool operator!=(const ContainerType&) const;
    bool operator<(const ContainerType&) const;
    bool operator>(const ContainerType&) const;

    // Arthemtic operators
    ContainerType& operator-=(const ValueType);
    ContainerType& operator+=(const ValueType);

    // Comparison operators
    bool operator==(const ValueType) const;
    bool operator<=(const ValueType) const;
    bool operator>=(const ValueType) const;
    bool operator!=(const ValueType) const;
    bool operator<(const ValueType) const;
    bool operator>(const ValueType) const;
};

typedef ContainerType<BasicTypeName::OrderId> OrderIdType;
typedef ContainerType<BasicTypeName::StockId> StockIdType;
typedef ContainerType<BasicTypeName::UserId> UserIdType;
typedef ContainerType<BasicTypeName::CompanyId> CompanyIdType;
typedef ContainerType<BasicTypeName::Amount> AmountType;
typedef ContainerType<BasicTypeName::Price> PriceType;

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
inline uint qHash(const ContainerType<TypeName, ValueType, defaultValue>& key, uint seed = 0) Q_DECL_NOTHROW
{
    return ::qHash(key.value, seed);
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultType>
QDataStream &operator<<(QDataStream& stream, const ContainerType<TypeName,ValueType,defaultType>&);
template<BasicTypeName TypeName, typename ValueType, ValueType defaultType>
QDataStream &operator>>(QDataStream& stream, ContainerType<TypeName,ValueType,defaultType>&);
template<BasicTypeName TypeName, typename ValueType, ValueType defaultType>
QDebug &operator<<(QDebug& stream, const ContainerType<TypeName,ValueType,defaultType>&);
template<BasicTypeName TypeName, typename ValueType, ValueType defaultType>
QDebug &operator>>(QDebug& stream, ContainerType<TypeName,ValueType,defaultType>&);


namespace Failure
{
typedef qint8 FailureTypeType;
enum FailureType : FailureTypeType
{
    BAD_PASSWORD,
    CANNOT_REGISTER_WHEN_LOGGED,
    BAD_USERID_OR_PASSWORD,
    NOT_LOGGED,
    ALREADY_LOGGED,
    INVALID_MESSAGE,
    UNRECOGNIZED_MESSAGE
};
}

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
typedef qint16 MessageLengthType;
typedef qint8 MessageTypeType;
enum MessageType : MessageTypeType
{
    /*0-19: komunikacja z serwerem, logowanie, etc*/

    REQUEST_REGISTER_USER = 0x00,
    RESPONSE_REGISTER_USER_SUCCESS,
    RESPONSE_OK,
    RESPONSE_FAILURE,
    REQUEST_LOGIN_USER,

    //RESPONSE_LOGIN_USER_SUCCESS,
    //RESPONSE_LOGIN_USER_FAILURE,
    //RESPONSE_UNRECOGNIZED_USER,             // Np. odpowiedz dla uzytkownika, gdy probuje
    // niezalogowany wykonac akcje wymagajaca zalogowania.


    /* 20-39: akcje i zlecenia */

    REQUEST_SELL_STOCK_ORDER = 0x14,        // Request zlecenie kupna
    REQUEST_BUY_STOCK_ORDER,                // Request zlecenie sprzedazy

    /*
         * Przestalem wiedziec co dokladnie robia 3 ponizsze wiadomosci.
         * Niestety (najprawdopodobniej moj) komentarz jest bardzo lakoniczny,
         * a fragment kodu w ktorych sa wykorzystywane wskazuje na kontintuicyjnosc nazw
         * :-(
         *                                      -- jam231
         */

    RESPONSE_BUY_TRANSACTION,               // Response wysylanie informacji o danej transakcji kupna
    RESPONSE_SELL_TRANSACTION,              // Response wysylanie informacji o danej transakcji sprzedazy


    RESPONSE_TRANSACTION_CHANGE,            // Response powiadomienie o zmianie (zrealizowaniu?) danej transakcji
    // wszystkich zasubskrybowanych do danego zasobu

    RESPONSE_SHOW_ORDER,                    // Response informacja o nowym zleceniu
    RESPONSE_SHOW_BEST_ORDER,               // Response informacja o nowym najlepszym zleceniu

    REQUEST_SUBSCRIBE_STOCK,                // Request subskrybuj
    REQUEST_UNSUBSCRIBE_STOCK,              // Request przestan subskrybowac


    REQUEST_GET_MY_STOCKS,                  // Request prosba o liste wlasnych zasobów
    RESPONSE_SHOW_USER_STOCKS,              // Response wysylanie listy zasobow
    REQUEST_GET_MY_ORDERS,                  // Request prosba o liste wlasnych zasobów
    RESPONSE_SHOW_USER_ORDERS,              // Response Odeslanie listy zasobow
    REQUEST_GET_STOCK_INFO,                 // Request prosba o info o danym zasobie
    RESPONSE_STOCK_INFO,                    // Response odpowiedz na prosbe o info

    REQUEST_CANCEL_ORDER,                   // Request żadanie anulowania zlecenia o podanym id.
    RESPONSE_ORDER_ID,


    /* 40-49: sesja, stan akcji */

    // Te sa zaimplementowane, ale nie uzywane --jam231

    REQUEST_COMPANY_STATUS = 0x28,
    RESPONSE_COMPANY_ACTIVE,
    RESPONSE_COMPANY_FROZEN,

    // Te sa w zawieszeniu, czekaja na implementacje lub zmiane --jam231

    RESPONSE_SESSION_OPEN,
    RESPONSE_SESSION_CLOSED,
    REQUEST_SESSION_STATUS,

    /* 50-99 */

    /* 100+ inne */
    MESSAGE_UNDEFINED = 0x64
};

NETWORKPROTOCOLSHARED_EXPORT MessageType toMessageType(MessageTypeType);
}

//                                                      TEMPLATE DEFINITIONS
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
QDataStream &operator<<(QDataStream& stream, const ContainerType<TypeName, ValueType, defaultValue>& containerType)
{
    stream << containerType.value;
    return stream;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
QDataStream &operator>>(QDataStream& stream, ContainerType<TypeName, ValueType, defaultValue>& containerType)
{
    stream >> containerType.value;
    return stream;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
QDebug &operator<<(QDebug& stream, const ContainerType<TypeName, ValueType, defaultValue>& containerType)
{
    stream << containerType.value;
    return stream;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
QDebug &operator>>(QDebug& stream, ContainerType<TypeName, ValueType, defaultValue>& containerType)
{
    stream >> containerType.value;
    return stream;
}
// Arthemtic operators
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
ContainerType<TypeName, ValueType, defaultValue>& ContainerType<TypeName, ValueType, defaultValue>::operator-=(const ContainerType<TypeName, ValueType, defaultValue>& rhs)
{
    return (*this) -= rhs.value;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
ContainerType<TypeName, ValueType, defaultValue>& ContainerType<TypeName, ValueType, defaultValue>::operator+=(const ContainerType<TypeName, ValueType, defaultValue>& rhs)
{
    return (*this) += rhs.value;
}

// Comparison operators
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator==(const ContainerType<TypeName, ValueType, defaultValue>& rhs) const
{
    return (*this) == rhs.value;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator>=(const ContainerType<TypeName, ValueType, defaultValue>& rhs) const
{
    return (*this) >= rhs.value;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator<=(const ContainerType<TypeName, ValueType, defaultValue>& rhs) const
{
    return (*this) <= rhs.value;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator!=(const ContainerType<TypeName, ValueType, defaultValue>& rhs) const
{
    return (*this) != rhs.value;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator>(const ContainerType<TypeName, ValueType, defaultValue>& rhs) const
{
    return (*this) > rhs.value;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator<(const ContainerType<TypeName, ValueType, defaultValue>& rhs) const
{
    return (*this) < rhs.value;
}
// Arthemtic operators
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
ContainerType<TypeName, ValueType, defaultValue>& ContainerType<TypeName, ValueType, defaultValue>::operator-=(const ValueType rhs)
{
    this->value -= rhs;
    return this;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
ContainerType<TypeName, ValueType, defaultValue>& ContainerType<TypeName, ValueType, defaultValue>::operator+=(const ValueType rhs)
{
    this->value += rhs;
    return this;
}

// Comparison operators
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator==(const ValueType rhs) const
{
    return this->value == rhs;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator>=(const ValueType rhs) const
{
    return this->value >= rhs;
}

template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator<=(const ValueType rhs) const
{
    return this->value <= rhs;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator!=(const ValueType rhs) const
{
    return this->value != rhs;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator>(const ValueType rhs) const
{
    return this->value > rhs;
}
template<BasicTypeName TypeName, typename ValueType, ValueType defaultValue>
bool ContainerType<TypeName, ValueType, defaultValue>::operator<(const ValueType rhs) const
{
    return this->value < rhs;
}

}

// Outside of Types namespace
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator>>(QDataStream& stream, Types::Message::MessageType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator>>(QDataStream& stream, Types::Order::OrderType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Types::Message::MessageType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Types::Order::OrderType&);



}
}
#endif // TYPES_H
