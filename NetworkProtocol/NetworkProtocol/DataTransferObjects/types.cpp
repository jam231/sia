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
    case RESPONSE_REGISTER_USER_OK : return RESPONSE_REGISTER_USER_OK;
    case RESPONSE_REGISTER_USER_FAIL : return RESPONSE_REGISTER_USER_FAIL;
    case LOGIN_USER_REQ : return LOGIN_USER_REQ;
    case LOGIN_USER_RESP_OK : return LOGIN_USER_RESP_OK;
    case LOGIN_USER_RESP_FAIL : return LOGIN_USER_RESP_FAIL;
    case SUBSCRIBE_STOCK : return SUBSCRIBE_STOCK;
    case UNSUBSCRIBE_STOCK : return UNSUBSCRIBE_STOCK;
    case SELL_STOCK_REQ : return SELL_STOCK_REQ;
    case BUY_STOCK_REQ : return BUY_STOCK_REQ;
    case GET_MY_STOCKS : return GET_MY_STOCKS;
    case GET_MY_STOCKS_RESP : return GET_MY_STOCKS_RESP;
    case GET_MY_ORDERS : return GET_MY_ORDERS;
    case GET_MY_ORDERS_RESP : return GET_MY_ORDERS_RESP;
    case GET_STOCK_INFO : return GET_STOCK_INFO;
    case CANCEL_ORDER_REQ : return CANCEL_ORDER_REQ;
    case GET_STOCK_INFO_RESP : return GET_STOCK_INFO_RESP;
    case ORDER_ID_RESP : return ORDER_ID_RESP;
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
