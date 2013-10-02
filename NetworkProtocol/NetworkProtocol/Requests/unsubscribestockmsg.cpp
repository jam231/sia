#include "unsubscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{
using namespace DTO;

UnsubscribeStockMsg::UnsubscribeStockMsg(QDataStream& in) : Request(in)
{
    in >> m_stockId;
}


Types::Message::MessageType UnsubscribeStockMsg::type() const
{
    return Types::Message::MessageType::UNSUBSCRIBE_STOCK;
}

Types::StockIdType UnsubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType UnsubscribeStockMsg::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId);
}

const QString UnsubscribeStockMsg::getMessageName()
{
    return Request::getMessageName() + " :: UnsubscribeStockMsg";
}

}
}
