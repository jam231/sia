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

    qint16 length() const;
    MessageType type() const;
    void send(QIODevice* connection);
};

}
}
#endif // LOGINUSERRESPOK_H
