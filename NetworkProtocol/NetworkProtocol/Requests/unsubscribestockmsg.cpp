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


Types::MessageType UnsubscribeStockMsg::type() const
{
    return Types::MessageType::UNSUBSCRIBE_STOCK;
}

Types::StockIdType UnsubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType UnsubscribeStockMsg::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(m_stockId);
}

}
}
