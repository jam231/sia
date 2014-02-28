#ifndef REGISTERUSERMSG_H
#define REGISTERUSERMSG_H

#include "request.h"

#include <QString>

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT RegisterUser : public Request
{
    QString _password;
public:
    RegisterUser(QDataStream& msg);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    QString getPassword() const;

};
}
}
#endif // REGISTERUSERMSG_H
