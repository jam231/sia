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

    qint16 length() const;
    MessageType type() const;
    void send(QIODevice* connection);
};

}
}
#endif // UNRECOGNIZEDUSERMSG_H
