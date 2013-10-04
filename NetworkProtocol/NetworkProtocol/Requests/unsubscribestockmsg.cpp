#include "unsubscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{
using namespace DTO;

UnsubscribeStock::UnsubscribeStock(QDataStream& in) : Request(in)
{
    in >> m_stockId;
}


Types::Message::MessageType UnsubscribeStock::type() const
{
    return Types::Message::MessageType::REQUEST_UNSUBSCRIBE_STOCK;
}

Types::StockIdType UnsubscribeStock::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType UnsubscribeStock::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId);
}

const QString UnsubscribeStock::getMessageName()
{
    return Request::getMessageName() + " :: UnsubscribeStock";
}

}
}
