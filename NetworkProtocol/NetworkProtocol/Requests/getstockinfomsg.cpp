#include "getstockinfomsg.h"

#include "networkprotocol_utilities.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetStockInfo::GetStockInfo(QDataStream& serialized_request)
    : GetStockInfo(std::move(GlobalUtilities::getLogger()), serialized_request)
{}

GetStockInfo::GetStockInfo(std::shared_ptr<AbstractLogger> logger,
                           QDataStream &serialized_request)
{
    if(serialized_request.device()->bytesAvailable() < sizeof(_stockId))
    {
        LOG_TRACE(logger,
                         QString("Malformed request: Not enough bytes in "\
                                 "serialized_request to stock id. Is %1 should be >%2.")
                        .arg(serialized_request.device()->bytesAvailable())
                        .arg(sizeof(_stockId)));

        throw MalformedRequestError("Not enough bytes in serialized_request to read"\
                               " stock id.");
    }

    serialized_request >> _stockId;

    if(_stockId <= 0)
    {
        LOG_TRACE(logger, QString("Invalid stock id. stockId(%1) <= 0.")
                          .arg(_stockId.value));

        throw InvalidRequestBodyError("stock id <= 0.");
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
    return Request::length() + sizeof(_stockId);
}


}
}
