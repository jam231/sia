#include "unsubscribestockmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

UnsubscribeStockMsg::UnsubscribeStockMsg(QDataStream& in) : Request(in)
{
    in >> m_stockId;
}

Message::MessageType UnsubscribeStockMsg::type() const
{
    return UNSUBSCRIBE_STOCK;
}

qint32 UnsubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

qint16 UnsubscribeStockMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_stockId);
}

}
}
