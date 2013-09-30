#include "getstockinfomsg.h"


namespace NetworkProtocol
{
namespace Requests
{

GetStockInfoMsg::GetStockInfoMsg(QDataStream &in) : Request(in)
{
    in >> m_stockId;
}

Message::MessageType GetStockInfoMsg::type() const
{
    return GET_STOCK_INFO;
}

qint32 GetStockInfoMsg::getStockId() const
{
    return m_stockId;
}

qint16 GetStockInfoMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_stockId);
}

}
}
