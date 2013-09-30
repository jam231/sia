#ifndef REGISTERUSERRESPFAIL_H
#define REGISTERUSERRESPFAIL_H

#include "response.h"

#include <QString>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT RegisterUserRespFail : public Response
{
    QString m_reason;

public:
    RegisterUserRespFail(QString reason);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // REGISTERUSERRESPFAIL_H
