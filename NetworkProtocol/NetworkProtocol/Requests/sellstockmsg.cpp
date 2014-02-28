#include "sellstockmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SellStock::SellStock(QDataStream& in) : Request(in)
{
    in >> _stockId;
    in >> _amount;
    in >> _price;

    // Mam wielka nadzieje, ze w bazie te rzeczy sa sprawdane,
    // a juz na pewno _stockId
    if(_amount <= 0 || _price <= 0)
    {
        qWarning() << "[] Wiadomośc nie spełnia: _amount <= 0 || _price <= 0";
        throw InvalidRequest();
    }
}

Types::Message::MessageType SellStock::type() const
{
    return Types::Message::MessageType::REQUEST_SELL_STOCK_ORDER;
}


DTO::Types::AmountType SellStock::getAmount() const
{
    return _amount;
}

DTO::Types::PriceType SellStock::getPrice() const
{
    return _price;
}

DTO::Types::StockIdType SellStock::getStockId() const
{
    return _stockId;
}

DTO::Types::Message::MessageLengthType SellStock::length() const
{
    return sizeof(Types::Message::MessageType) + sizeof(_stockId) +
            sizeof(_price) + sizeof(_amount);
}



}
}
