#ifndef FAILUREMSG_H
#define FAILUREMSG_H


#include "response.h"

#include <QString>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT Failure : public Response
{
    DTO::Types::Failure::FailureType _reason;

public:
    Failure(DTO::Types::Failure::FailureType);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice* connection);
};

}
}
#endif // FAILUREMSG_H
