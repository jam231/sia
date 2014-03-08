#ifndef FAILUREMSG_H
#define FAILUREMSG_H


#include "response.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: Failure
 *
 *  This message is for notifying about failure of request.
 *  Message contains error number.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><failure_type : 1>
 *
 *  where
 *      message_type         = Message::RESPONSE_FAILURE (0x3)
 */
class NETWORKPROTOCOLSHARED_EXPORT Failure : public Response
{
    DTO::Types::Failure::FailureType _reason;

public:
    Failure(DTO::Types::Failure::FailureType);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice* connection);
};

}
}
#endif // FAILUREMSG_H
