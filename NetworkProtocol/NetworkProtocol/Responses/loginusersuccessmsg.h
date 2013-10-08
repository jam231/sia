#ifndef LOGINUSERSUCCESSMSG_H
#define LOGINUSERSUCCESSMSG_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT LoginUserSuccess : public Response
{

public:
    LoginUserSuccess();

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // LOGINUSERSUCCESSMSG_H
