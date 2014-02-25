#include "sellstockmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SellStock::SellStock(QDataStream& in) : Request(in)
{
    in >> m_stockId;
    in >> m_amount;
    in >> m_price;

    // Mam wielka nadzieje, ze w bazie te rzeczy sa sprawdane,
    // a juz na pewno m_stockId
    if(m_amount <= 0 || m_price <= 0)
    {
        qWarning() << "[] Wiadomośc nie spełnia: m_amount <= 0 || m_price <= 0";
        throw InvalidRequest();
    }
}

Types::Message::MessageType SellStock::type() const
{
    return Types::Message::MessageType::REQUEST_SELL_STOCK_ORDER;
}


DTO::Types::AmountType SellStock::getAmount() const
{
    return m_amount;
}

DTO::Types::PriceType SellStock::getPrice() const
{
    return m_price;
}

DTO::Types::StockIdType SellStock::getStockId() const
{
    return m_stockId;
}

DTO::Types::Message::MessageLengthType SellStock::length() const
{
    return sizeof(Types::Message::MessageType) + sizeof(m_stockId) +
            sizeof(m_price) + sizeof(m_amount);
}



}
}
