#ifndef LOGINUSERRESPOK_H
#define LOGINUSERRESPOK_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT LoginUserRespOk : public Response
{

public:
    LoginUserRespOk();

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // LOGINUSERRESPOK_H
