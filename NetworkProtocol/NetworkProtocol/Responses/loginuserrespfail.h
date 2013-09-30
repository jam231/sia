#ifndef LOGINUSERRESPFAIL_H
#define LOGINUSERRESPFAIL_H


#include "response.h"

#include <QString>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT LoginUserRespFail : public Response
{
    QString m_reason;

public:
    LoginUserRespFail(QString);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    void send(QIODevice* connection);
};

}
}
#endif // LOGINUSERRESPFAIL_H
