#ifndef REGISTERUSERMSG_H
#define REGISTERUSERMSG_H

#include "request.h"

#include <QString>

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: RegisterUser
 *
 *  This message is for requesting registration of new account.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><password_str_length : 2><password_str : password_str_length>
 *
 *  where
 *      message_type         = MessageType::REQUEST_REGISTER_USER
 */
class NETWORKPROTOCOLSHARED_EXPORT RegisterUser : public Request
{
    QString _password;
public:
    RegisterUser(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    QString getPassword() const;

};
}
}
#endif // REGISTERUSERMSG_H
