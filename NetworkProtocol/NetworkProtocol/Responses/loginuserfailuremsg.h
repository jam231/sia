#ifndef LOGINUSERRESPFAIL_H
#define LOGINUSERRESPFAIL_H


#include "response.h"

#include <QString>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT LoginuserFailure : public Response
{
    QString m_reason;

public:
    LoginuserFailure(QString);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // LOGINUSERRESPFAIL_H
