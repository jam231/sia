#ifndef REGISTERUSERSUCCESSMSG_H
#define REGISTERUSERSUCCESSMSG_H

#include "response.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: RegisterUserSuccess
 *
 *  This message is for sending confirmation of registration.
 *  Message contains newly registered user's id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><user_id : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_REGISTER_USER_SUCCESS (0x1)
 */
class NETWORKPROTOCOLSHARED_EXPORT RegisterUserSuccess : public Response
{
    DTO::Types::UserIdType _userId;

public:
    RegisterUserSuccess(std::shared_ptr<AbstractLogger>,
                        DTO::Types::UserIdType);
    RegisterUserSuccess(DTO::Types::UserIdType);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::UserIdType getUserId() const;


    void send(QIODevice* connection);
};

}
}
#endif // REGISTERUSERSUCCESSMSG_H
