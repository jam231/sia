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

template<BasicTypeName TypeName, typename ValueType = quint32, ValueType defaultValue = ValueType{}>
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

typedef ContainerType<BasicTypeName::OrderId, quint32> OrderIdType;
typedef ContainerType<BasicTypeName::StockId, quint32> StockIdType;
typedef ContainerType<BasicTypeName::UserId, quint32> UserIdType;
typedef ContainerType<BasicTypeName::CompanyId, quint32> CompanyIdType;
typedef ContainerType<BasicTypeName::Amount, quint32> AmountType;
typedef ContainerType<BasicTypeName::Price, quint32> PriceType;

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
typedef quint8 FailureTypeType;
enum FailureType : FailureTypeType
{
    NO_FAILURE              = 0x0,
    BAD_USERID_OR_PASSWORD,
    NOT_AUTHORIZED,
    ALREADY_LOGGED,
    RESOURCE_NOT_AVAILABLE,         // When user requests order for nonexistent stock, or wants to cancel order
                                    // he doesn't have.
    INSUFFICIENT_FUNDS,             // When user requests BUY order he can't afford
                                    // or SELL order for stock he doesn't have (or have too litle) -- (Red Alert wtf!)
    MALFORMED_MESSAGE,              // Lengths don't match
    INVALID_MESSAGE_BODY,           // Contents are well-formed, but violate constraints
    UNRECOGNIZED_MESSAGE,            // Message type is not defined.
    REQUEST_DROPPED
};

NETWORKPROTOCOLSHARED_EXPORT FailureType toFailureType(FailureTypeType);
}

// Force separation
namespace Order
{
typedef quint8 OrderTypeType;
enum OrderType : OrderTypeType
{
    UNDEFINED = 0x0,
    BUY,
    SELL
};

NETWORKPROTOCOLSHARED_EXPORT OrderType toOrderType(OrderTypeType);
}

namespace Company
{
    typedef quint8 CompanyStatusTypeType;
    enum CompanyStatusType : CompanyStatusTypeType
    {
        UNDEFINED = 0x0,
        ACTIVE,
        FROZEN
    };
    NETWORKPROTOCOLSHARED_EXPORT CompanyStatusType toCompanyStatusType(CompanyStatusTypeType);
}

// Force separation
namespace Message
{
typedef quint16 MessageLengthType;
typedef quint8 MessageTypeType;
enum MessageType : MessageTypeType
{
    /*0-19: login, register, etc. */

    REQUEST_REGISTER_USER = 0x00,           // Requests::RegisterUser
    RESPONSE_REGISTER_USER_SUCCESS,         // Responses::RegisterUserSuccess
    RESPONSE_OK,                            // Responses::Ok
    RESPONSE_FAILURE,                       // Responses::Failure
    REQUEST_LOGIN_USER,


    /* 20-39: stocks & orders */

    REQUEST_SELL_STOCK_ORDER = 0x14,        // Requests::SellStock
    REQUEST_BUY_STOCK_ORDER,                // Requests::BuyStock


    RESPONSE_ORDER_COMPLETED,               // Responses::OrderCompleted
    RESPONSE_ORDER_REALIZATION,             // Responses::OrderRealization


    RESPONSE_LAST_TRANSACTION,              // Responses::LastTransaction


    RESPONSE_SHOW_NEW_ORDER,                // Responses::ShowNewOrder
    RESPONSE_SHOW_BEST_ORDER,               // Responses::ShowBestOrder

    REQUEST_SUBSCRIBE_STOCK,                // Requests::SubscribeStock
    REQUEST_UNSUBSCRIBE_STOCK,              // Requests::UnsubscribeStock


    REQUEST_GET_MY_STOCKS,                  // Requests::GetMyStocks
    RESPONSE_LIST_OF_STOCKS,                // Responses::ListOfStocks
    REQUEST_GET_MY_ORDERS,                  // Requests::GetMyOrders
    RESPONSE_LIST_OF_ORDERS,                // Responses::ListOfOrders
    REQUEST_GET_STOCK_INFO,                 // Requests::GetStockInfo
    RESPONSE_STOCK_INFO,                    // Responses::StockInfo
    REQUEST_CANCEL_ORDER,                   // Requests::CancelOrder

    RESPONSE_ORDER_ACCEPTED,                // Responses::OrderAccepted

    RESPONSE_SHOW_NO_BEST_ORDER,            // Response::ShowNoBestOrder
    /* 40-49: session, company  */

    // Implemented but not used.

    REQUEST_GET_COMPANY_STATUS = 0x28,      // Requests::GetCompanyStatus
    RESPONSE_COMPANY_STATUS,                // Response::CompanyStatus


    // Not implemented

    REQUEST_GET_SESSION_STATUS,
    RESPONSE_SESSION_STATUS,


    /* 50-99 */

    /* 100+ others */
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
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator>>(QDataStream& stream, Types::Company::CompanyStatusType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator>>(QDataStream& stream, Types::Failure::FailureType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Types::Failure::FailureType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Types::Message::MessageType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Types::Order::OrderType&);
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Types::Company::CompanyStatusType&);


}
}
#endif // TYPES_H
