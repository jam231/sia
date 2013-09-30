#ifndef UNRECOGNIZEDUSERMSG_H
#define UNRECOGNIZEDUSERMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT UnrecognizedUserMsg : public Response
{
public:

    UnrecognizedUserMsg();

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // UNRECOGNIZEDUSERMSG_H
