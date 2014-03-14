#include "sellstockmsg.h"

#include "networkprotocol_utilities.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SellStock::SellStock(QDataStream& serialized_request)
    : SellStock(std::move(GlobalUtilities::getLogger()), serialized_request)
{}


SellStock::SellStock(std::shared_ptr<AbstractLogger> logger,
                     QDataStream& serialized_request)
{
    if(serialized_request.device()->bytesAvailable() != sizeof(_stockId) +
                                                        sizeof(_amount) + sizeof(_price))
    {
        LOG_TRACE(logger,
                  QString("Malformed request: Not enough bytes in serialized_request"\
                          " to read stock id, amount and price. Is %1 should be %2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(_stockId) + sizeof(_amount) + sizeof(_price)));
        throw MalformedRequest("Wrong number bytes in serialized_request for"\
                               " stock id.");
    }
    serialized_request >> _stockId;
    serialized_request >> _amount;
    serialized_request >> _price;
    if(_stockId <= 0 || _amount <= 0 || _price <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid request body: stockId(%1) <= 0 || amount(%2) <= 0 "\
                          "|| price(%3) <= 0.")
                  .arg(_stockId.value).arg(_amount.value)
                  .arg(_price.value));

        throw InvalidRequestBody("One of stockId, amount, price is <= 0.");
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
    return Request::length() +
           sizeof(_stockId) + sizeof(_price) + sizeof(_amount);
}



}
}
