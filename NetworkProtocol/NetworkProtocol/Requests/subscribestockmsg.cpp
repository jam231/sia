#include "subscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SubscribeStock::SubscribeStock (QDataStream& in) : Request(in)
{
    in >> m_stockId;
}

Types::Message::MessageType SubscribeStock::type() const
{
    return Types::Message::MessageType::REQUEST_SUBSCRIBE_STOCK;
}

Types::StockIdType SubscribeStock::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType SubscribeStock::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId);
}

const QString SubscribeStock::getMessageName()
{
    return Request::getMessageName() + " :: SubscribeStock";
}

}
}
