#ifndef REGISTERUSERSUCCESSMSG_H
#define REGISTERUSERSUCCESSMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT RegisterUserSuccess : public Response
{
    DTO::Types::UserIdType m_userId;

public:
    RegisterUserSuccess(DTO::Types::UserIdType userId);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::UserIdType getUserId() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // REGISTERUSERSUCCESSMSG_H
