#include "sellstockreqmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

SellStockReqMsg::SellStockReqMsg(QDataStream& in) : Request(in)
{
    in >> m_stockId;
    in >> m_amount;
    in >> m_price;

    // Mam wielka nadzieje, ze w bazie te rzeczy sa sprawdane,
    // a juz na pewno m_stockId
    if(m_amount <= 0 || m_price <= 0)
    {
        qDebug() << "[" << getMessageName() << "] m_amount <= 0 || m_price <= 0";
        throw InvalidRequest();
    }
}

Message::MessageType SellStockReqMsg::type() const
{
    return SELL_STOCK_REQ;
}


qint32 SellStockReqMsg::getAmount() const
{
    return m_amount;
}

qint32 SellStockReqMsg::getPrice() const
{
    return m_price;
}

qint32 SellStockReqMsg::getStockId() const
{
    return m_stockId;
}

qint16 SellStockReqMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_stockId) +
            sizeof(m_price) + sizeof(m_amount);
}

}
}
