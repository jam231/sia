#include "getstockinfomsg.h"

#include "networkprotocol_utilities.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetStockInfo::GetStockInfo(QDataStream &serialized_request)
{
    if(serialized_request.device()->bytesAvailable() < sizeof(_stockId))
    {
        GLOBAL_LOG_TRACE(QString("Malformed request: Not enough bytes in serialized_request"\
                          " to stock id. Is %1 should be >%2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(_stockId)));
        throw MalformedRequest("Not enough bytes in serialized_request to read"\
                               " stock id.");
    }

    serialized_request >> _stockId;

    if(_stockId <= 0)
    {
        GLOBAL_LOG_TRACE(QString("Invalid stock id. stockId(%1) <= 0.")
                  .arg(_stockId.value));
        throw InvalidRequestBody("stock id <= 0.");
    }
}

Types::Message::MessageType GetStockInfo::type() const
{
    return Types::Message::MessageType::REQUEST_GET_STOCK_INFO;
}

DTO::Types::StockIdType GetStockInfo::getStockId() const
{
    return _stockId;
}

Types::Message::MessageLengthType GetStockInfo::length() const
{
    return sizeof(_stockId);
}


}
}
