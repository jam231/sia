#ifndef MESSAGE_H
#define MESSAGE_H

#include "networkprotocol_global.h"
#include "DataTransferObjects/types.h"
#include <QString>

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT Message
{  
public:
    // Length of the message in BYTES
    virtual DTO::Types::MessageLengthType length() const = 0;
    virtual DTO::Types::MessageType type() const = 0;

    static QString getMessageName();

    virtual ~Message() {}
};
}
#endif // MESSAGE_H
