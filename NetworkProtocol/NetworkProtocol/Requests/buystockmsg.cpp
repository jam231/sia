#include "buystockmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

BuyStock::BuyStock(QDataStream& in) : Request(in)
{   
    in >> _stockId;
    in >> _amount;
    in >> _price;

    if(_amount <= 0 || _price <= 0)
    {
        qWarning() << "[] Wiadomośc nie spełnia: _amount <= 0 || _price <= 0";
        throw InvalidRequest();
    }
}

Types::Message::MessageType BuyStock::type() const
{
    return Types::Message::MessageType::REQUEST_BUY_STOCK_ORDER;
}

Types::AmountType BuyStock::getAmount() const
{
    return _amount;
}

Types::PriceType BuyStock::getPrice() const
{
    return _price;
}


Types::StockIdType BuyStock::getStockId() const
{
    return _stockId;
}

Types::Message::MessageLengthType BuyStock::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(_stockId) + sizeof(_price)  + sizeof(_amount);
}

}
}
