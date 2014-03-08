#ifndef OKMSG_H
#define OKMSG_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: Ok
 *
 *  This message is for sending confirmation of request acceptance.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1>
 *
 *  where
 *      message_type         = Message::RESPONSE_OK (0x2)
 */
class NETWORKPROTOCOLSHARED_EXPORT Ok : public Response
{

public:
    Ok();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice* connection);
};

}
}
#endif // OKMSG_H
