#ifndef UNRECOGNIZEDUSERMSG_H
#define UNRECOGNIZEDUSERMSG_H

#include "omessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT UnrecognizedUserMsg : public OMessage
{

    qint16 length() const;
public:
    UnrecognizedUserMsg();
    MessageType type() const;
    void send(QIODevice* connection);
};

}
#endif // UNRECOGNIZEDUSERMSG_H
