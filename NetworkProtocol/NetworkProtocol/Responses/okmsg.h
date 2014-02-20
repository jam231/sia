#ifndef OkMSG_H
#define OkMSG_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT Ok : public Response
{

public:
    Ok();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // OkMSG_H
