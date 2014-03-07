#include "message.h"


namespace NetworkProtocol {


DTO::Types::Message::MessageLengthType Message::length() const
{
    return sizeof(DTO::Types::Message::MessageLengthType) +
           sizeof(DTO::Types::Message::MessageType);
}
}
