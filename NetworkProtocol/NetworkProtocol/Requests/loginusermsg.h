#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "request.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: LoginUser
 *
 *  This message is for requesting log in.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><user_id : 4><pswd_utf8_bytes_length : 2><pswd_utf8_bytes : pswd_utf8_bytes_length>
 *
 *  where
 *      message_type         = MessageType::REQUEST_LOGIN_USER (0x4)
 */
class NETWORKPROTOCOLSHARED_EXPORT LoginUser : public Request
{
    DTO::Types::UserIdType _userId;
    QString _password;

public:
    LoginUser(std::shared_ptr<AbstractLogger>, QDataStream&);
    LoginUser(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::UserIdType getUserId() const;
    QString getUserPassword() const;

};
}
}
#endif // LOGINUSERMSG_H
