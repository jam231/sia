#ifndef UNRECOGNIZEDUSERMSG_H
#define UNRECOGNIZEDUSERMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT UnrecognizedUser : public Response
{
public:

    UnrecognizedUser();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // UNRECOGNIZEDUSERMSG_H
