#include "buystockreqmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

BuyStockReqMsg::BuyStockReqMsg(QDataStream& in) : Request(in)
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

Message::MessageType BuyStockReqMsg::type() const
{
    return BUY_STOCK_REQ;
}

qint32 BuyStockReqMsg::getAmount() const
{
    return m_amount;
}

qint32 BuyStockReqMsg::getPrice() const
{
    return m_price;
}

qint32 BuyStockReqMsg::getStockId() const
{
    return m_stockId;
}

qint16 BuyStockReqMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_stockId) +
           sizeof(m_price)  + sizeof(m_amount);
}

}
}
