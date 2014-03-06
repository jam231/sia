#ifndef GETSTOCKINFOMSG_H
#define GETSTOCKINFOMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: GetStockInfo
 *
 *  This message is for requesting information about stock with given id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_GET_STOCK_INFO (0x21)
 */
class NETWORKPROTOCOLSHARED_EXPORT GetStockInfo : public Request
{
    DTO::Types::StockIdType _stockId;
public:
    GetStockInfo(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

};
}
}
#endif // GETSTOCKINFOMSG_H
