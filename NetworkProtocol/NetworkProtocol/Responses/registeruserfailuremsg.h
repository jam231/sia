#ifndef REGISTERUSEPFAILUREMSG_H
#define REGISTERUSERFAILUREMSG_H

#include "response.h"

#include <QString>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT RegisterUserFailure : public Response
{
    QString m_reason;

public:
    RegisterUserFailure(QString reason);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // REGISTERUSERFAILUREMSG_H
