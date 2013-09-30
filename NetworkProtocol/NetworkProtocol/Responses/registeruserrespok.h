#ifndef REGISTERUSERRESPMSG_H
#define REGISTERUSERRESPMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT RegisterUserRespOk : public Response
{
    qint32 m_userId;

public:
    RegisterUserRespOk(qint32 userId);

    qint16 length() const;
    MessageType type() const;
    qint32 getUserId() const;
    void send(QIODevice* connection);
};

}
}
#endif // REGISTERUSERRESPMSG_H
