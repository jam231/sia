#include "buystockmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

BuyStock::BuyStock(QDataStream& in) : Request(in)
{   
    in >> m_stockId;
    in >> m_amount;
    in >> m_price;

    if(m_amount <= 0 || m_price <= 0)
    {
        qWarning() << "[" << getMessageName() << "] Wiadomośc nie spełnia: m_amount <= 0 || m_price <= 0";
        throw InvalidRequest();
    }
}

Types::Message::MessageType BuyStock::type() const
{
    return Types::Message::MessageType::REQUEST_BUY_STOCK_ORDER;
}

Types::AmountType BuyStock::getAmount() const
{
    return m_amount;
}

Types::PriceType BuyStock::getPrice() const
{
    return m_price;
}

const QString BuyStock::getMessageName()
{
    return Request::getMessageName() + " :: BuyStock";
}

Types::StockIdType BuyStock::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType BuyStock::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId) + sizeof(m_price)  + sizeof(m_amount);
}

}
}
