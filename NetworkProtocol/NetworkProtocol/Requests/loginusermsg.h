#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: LoginUser
 *
 *  This message is for requesting log in.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><user_id : 4><password_str_length : 2><password_str : password_str_length>
 *
 *  where
 *      message_type         = MessageType::REQUEST_REGISTER_USER
 */
class NETWORKPROTOCOLSHARED_EXPORT LoginUser : public Request
{
    DTO::Types::UserIdType _userId;
    QString _password;

public:
    LoginUser(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::UserIdType getUserId() const;
    QString getUserPassword() const;

};
}
}
#endif // LOGINUSERMSG_H
