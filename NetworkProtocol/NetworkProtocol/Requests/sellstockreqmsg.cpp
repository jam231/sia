#include "sellstockreqmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

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

Types::Message::MessageType SellStockReqMsg::type() const
{
    return Types::Message::MessageType::SELL_STOCK_REQ;
}


DTO::Types::AmountType SellStockReqMsg::getAmount() const
{
    return m_amount;
}

DTO::Types::PriceType SellStockReqMsg::getPrice() const
{
    return m_price;
}

DTO::Types::StockIdType SellStockReqMsg::getStockId() const
{
    return m_stockId;
}

DTO::Types::MessageLengthType SellStockReqMsg::length() const
{
    return sizeof(Types::Message::MessageType) + sizeof(m_stockId) +
            sizeof(m_price) + sizeof(m_amount);
}

const QString SellStockReqMsg::getMessageName()
{
    return Request::getMessageName() + " :: SellStockReqMsg";
}

}
}
