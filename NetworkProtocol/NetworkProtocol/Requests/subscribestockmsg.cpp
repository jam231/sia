#include "subscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{

SubscribeStockMsg::SubscribeStockMsg(QDataStream& in) : Request(in)
{
    in >> m_stockId;
}

Message::MessageType SubscribeStockMsg::type() const
{
    return SUBSCRIBE_STOCK;
}

qint32 SubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

qint16 SubscribeStockMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_stockId);
}

}
}
