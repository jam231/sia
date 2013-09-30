#include "buystockreqmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

BuyStockReqMsg::BuyStockReqMsg(QDataStream& in) : Request(in)
{   
    in >> m_stockId;
    in >> m_amount;
    in >> m_price;

    if(m_amount <= 0 || m_price <= 0)
    {
        qDebug() << "[" << getMessageName() << "] m_amount <= 0 || m_price <= 0";
        throw InvalidRequest();
    }
}

Types::MessageType BuyStockReqMsg::type() const
{
    return Types::MessageType::BUY_STOCK_REQ;
}

Types::AmountType BuyStockReqMsg::getAmount() const
{
    return m_amount;
}

Types::PriceType BuyStockReqMsg::getPrice() const
{
    return m_price;
}

const QString BuyStockReqMsg::getMessageName()
{
    return Request::getMessageName() + " :: BuyStockReqMsg";
}

Types::StockIdType BuyStockReqMsg::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType BuyStockReqMsg::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(m_stockId) + sizeof(m_price)  + sizeof(m_amount);
}

}
}
