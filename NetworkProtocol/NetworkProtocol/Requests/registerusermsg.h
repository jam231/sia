#ifndef REGISTERUSERMSG_H
#define REGISTERUSERMSG_H

#include "request.h"

#include <QString>

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT RegisterUser : public Request
{
    QString m_password;
public:
    RegisterUser(QDataStream& msg);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    QString getPassword() const;

    static const QString getMessageName();
};
}
}
#endif // REGISTERUSERMSG_H
