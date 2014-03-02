#ifndef CANCELORDERMSG_H
#define CANCELORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: CancelOrder
 *
 *  This message is for requesting cancelation of order with given id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_CANCEL_ORDER
 */class NETWORKPROTOCOLSHARED_EXPORT CancelOrder : public Request
{
    DTO::Types::OrderIdType _orderId;

public:
    CancelOrder(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::OrderIdType getOrderId() const;

};
}
}
#endif // CANCELORDERMSG_H
