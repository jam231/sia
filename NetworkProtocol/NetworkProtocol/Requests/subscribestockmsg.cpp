#include "subscribestockmsg.h"

#include "networkprotocol_utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SubscribeStock::SubscribeStock(QDataStream& serialized_request)
    : SubscribeStock(std::move(GlobalUtilities::getLogger()), serialized_request)
{}

SubscribeStock::SubscribeStock(std::shared_ptr<AbstractLogger> logger,
                               QDataStream& serialized_request)
{
    if(serialized_request.device()->bytesAvailable() != sizeof(_stockId))
    {
        LOG_TRACE(logger,
                  QString("Malformed request: Wrong number of bytes in "\
                          "serialized_request to read stock id. Is %1 should be %2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(_stockId)));
        throw MalformedRequest("Wrong number bytes in serialized_request for"\
                               " stock id.");
    }
    serialized_request >> _stockId;
    if(_stockId <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid request body: stockId(%1) <= 0")
                  .arg(_stockId.value));
        throw InvalidRequestBody("stockId <= 0.");
    }
}

Types::Message::MessageType SubscribeStock::type() const
{
    return Types::Message::MessageType::REQUEST_SUBSCRIBE_STOCK;
}

Types::StockIdType SubscribeStock::getStockId() const
{
    return _stockId;
}

Types::Message::MessageLengthType SubscribeStock::length() const
{
    return Request::length() +
           sizeof(_stockId);
}


}
}
