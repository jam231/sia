#ifndef GETMYSTOCKSMSG_H
#define GETMYSTOCKSMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: GetMyStocks
 *
 *  This message is for requesting list of owned stock.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_GET_MY_STOCKS
 */
class NETWORKPROTOCOLSHARED_EXPORT GetMyStocks : public Request
{
public:
    GetMyStocks(QDataStream&);
    GetMyStocks();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

};
}
}
#endif // GETMYSTOCKSMSG_H
