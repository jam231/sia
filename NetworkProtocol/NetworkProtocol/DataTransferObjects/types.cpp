#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

QDataStream &operator>>(QDataStream& stream, Types::Order::OrderType& orderType)
{
    Types::Order::OrderTypeType orderTypeType;
    stream >> orderTypeType;
    orderType = Types::Order::toOrderType(orderTypeType);
    return stream;
}


QDataStream &operator>>(QDataStream& stream, Types::Message::MessageType& messageType)
{
    Types::Message::MessageTypeType messageTypeType;
    stream >> messageTypeType;
    messageType = Types::Message::toMessageType(messageTypeType);
    return stream;
}

namespace Types
{

//                  OrderType

namespace Order
{
OrderType toOrderType(OrderTypeType type)
{
    switch(type)
    {
    case BUY: return BUY;
    case SELL: return SELL;
    default: return UNDEFINED;
    };
}
}

//                  MessageType

namespace Message
{
MessageType toMessageType(MessageTypeType msgId)
{
    switch(msgId)
    {
    case REQUEST_REGISTER_USER : return REQUEST_REGISTER_USER;
    case RESPONSE_REGISTER_USER_SUCCESS : return RESPONSE_REGISTER_USER_SUCCESS;
    case RESPONSE_REGISTER_USER_FAILURE : return RESPONSE_REGISTER_USER_FAILURE;
    case REQUEST_LOGIN_USER : return REQUEST_LOGIN_USER;
    case RESPONSE_LOGIN_USER_SUCCESS : return RESPONSE_LOGIN_USER_SUCCESS;
    case RESPONSE_LOGIN_USER_FAILURE : return RESPONSE_LOGIN_USER_FAILURE;
    case REQUEST_SUBSCRIBE_STOCK : return REQUEST_SUBSCRIBE_STOCK;
    case REQUEST_UNSUBSCRIBE_STOCK : return REQUEST_UNSUBSCRIBE_STOCK;
    case REQUEST_SELL_STOCK_ORDER : return REQUEST_SELL_STOCK_ORDER;
    case REQUEST_BUY_STOCK_ORDER : return REQUEST_BUY_STOCK_ORDER;
    case REQUEST_GET_MY_STOCKS : return REQUEST_GET_MY_STOCKS;
    case RESPONSE_SHOW_USER_STOCKS : return RESPONSE_SHOW_USER_STOCKS;
    case REQUEST_GET_MY_ORDERS : return REQUEST_GET_MY_ORDERS;
    case RESPONSE_SHOW_USER_ORDERS : return RESPONSE_SHOW_USER_ORDERS;
    case REQUEST_GET_STOCK_INFO : return REQUEST_GET_STOCK_INFO;
    case REQUEST_CANCEL_ORDER : return REQUEST_CANCEL_ORDER;
    case RESPONSE_STOCK_INFO : return RESPONSE_STOCK_INFO;
    case RESPONSE_ORDER_ID : return RESPONSE_ORDER_ID;
    default : return MESSAGE_UNDEFINED;
    };
}
}

OrderIdType::OrderIdType() : ContainerType<qint32>(0)
{
}
OrderIdType::OrderIdType(qint32 _value) : ContainerType<qint32>(_value)
{
}

StockIdType::StockIdType() : ContainerType<qint32>(0)
{
}
StockIdType::StockIdType(qint32 _value) : ContainerType<qint32>(_value)
{
}

UserIdType::UserIdType() : ContainerType<qint32>(0)
{
}
UserIdType::UserIdType(qint32 _value) : ContainerType<qint32>(_value)
{
}

CompanyIdType::CompanyIdType() : ContainerType<qint32>(0)
{
}
CompanyIdType::CompanyIdType(qint32 _value) : ContainerType<qint32>(_value)
{
}

AmountType::AmountType() : ContainerType<qint32>(0)
{
}
AmountType::AmountType(qint32 _value) : ContainerType<qint32>(_value)
{
}

PriceType::PriceType() : ContainerType<qint32>(0)
{
}
PriceType::PriceType(qint32 _value) : ContainerType<qint32>(_value)
{
}

}
}
}
