#ifndef GETMYSTOCKSMSG_H
#define GETMYSTOCKSMSG_H

#include "request.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: GetMyStocks
 *
 *  This message is for requesting list of owned stocks.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1>
 *
 *  where
 *      message_type         = MessageType::REQUEST_GET_MY_STOCKS (0x1D)
 */
class NETWORKPROTOCOLSHARED_EXPORT GetMyStocks : public Request
{
public:
    GetMyStocks(std::shared_ptr<AbstractLogger>, QDataStream&);
    GetMyStocks(QDataStream&);
    GetMyStocks();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

};
}
}
#endif // GETMYSTOCKSMSG_H
