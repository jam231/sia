#ifndef REGISTERUSERRESPMSG_H
#define REGISTERUSERRESPMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT RegisterUserRespOk : public Response
{
    DTO::Types::UserIdType m_userId;

public:
    RegisterUserRespOk(DTO::Types::UserIdType userId);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;
    DTO::Types::UserIdType getUserId() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // REGISTERUSERRESPMSG_H
