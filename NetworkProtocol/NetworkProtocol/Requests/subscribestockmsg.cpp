#include "subscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SubscribeStockMsg::SubscribeStockMsg(QDataStream& in) : Request(in)
{
    in >> m_stockId;
}

Types::Message::MessageType SubscribeStockMsg::type() const
{
    return Types::Message::MessageType::SUBSCRIBE_STOCK;
}

Types::StockIdType SubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType SubscribeStockMsg::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId);
}

const QString SubscribeStockMsg::getMessageName()
{
    return Request::getMessageName() + " :: SubscribeStockMsg";
}

}
}
