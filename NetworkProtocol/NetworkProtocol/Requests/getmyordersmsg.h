#ifndef GETMYORDERMSG_H
#define GETMYORDERMSG_H

#include "request.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: GetMyOrders
 *
 *  This message is for requesting list of user's pending orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1>
 *
 *  where
 *      message_type         = MessageType::REQUEST_GET_MY_ORDERS (0x1F)
 */
class NETWORKPROTOCOLSHARED_EXPORT GetMyOrders : public Request
{
public:
    GetMyOrders(std::shared_ptr<AbstractLogger>, QDataStream&);
    GetMyOrders(QDataStream&);
    GetMyOrders();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

};
}
}
#endif // GETMYORDERMSG_H
