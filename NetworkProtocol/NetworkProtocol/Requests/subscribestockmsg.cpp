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

Types::MessageType SubscribeStockMsg::type() const
{
    return Types::MessageType::SUBSCRIBE_STOCK;
}

DTO::Types::StockIdType SubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

DTO::Types::MessageLengthType SubscribeStockMsg::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(m_stockId);
}

}
}
