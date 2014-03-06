#ifndef MESSAGE_H
#define MESSAGE_H

#include "networkprotocol_global.h"
#include "DataTransferObjects/types.h"
#include <QString>

namespace NetworkProtocol {


/*
 *  Abstract class Message
 *
 *  Base for all messages (responses & requests).
 *
 *  Every message has the following format:
 *  <message_length : 2><message_type : 1><body : message_length>
 *
 */
class NETWORKPROTOCOLSHARED_EXPORT Message
{  
public:
    /*
     * Length of the *whole* message in BYTES (byte == 8 bits).
     */
    virtual DTO::Types::Message::MessageLengthType length() const = 0;
    virtual DTO::Types::Message::MessageType type() const = 0;

    virtual ~Message() {}
};
}
#endif // MESSAGE_H
