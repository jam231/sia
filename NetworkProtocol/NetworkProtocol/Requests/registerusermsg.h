#ifndef REGISTERUSERMSG_H
#define REGISTERUSERMSG_H

#include "request.h"

#include <QString>

#include "utilities.h"

#include <memory>

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: RegisterUser
 *
 *  This message is for requesting registration of new account.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><pswd_utf8_bytes_length : 2><pswd_utf8_bytes : pswd_utf8_bytes_length>
 *
 *  where
 *      message_type         = MessageType::REQUEST_REGISTER_USER
 */
class NETWORKPROTOCOLSHARED_EXPORT RegisterUser : public Request
{
    QString _password;
public:
    RegisterUser(std::shared_ptr<AbstractLogger>, QDataStream&);
    RegisterUser(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    QString getPassword() const;

};
}
}
#endif // REGISTERUSERMSG_H
