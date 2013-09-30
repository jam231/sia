#ifndef Response_H
#define Response_H

#include "message.h"

#include <QIODevice>
#include <QDataStream>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT Response : public Message
{
protected:
    void sendHeader(QDataStream& out);
public:
    static const QString getMessageName();
    virtual void send(QIODevice* connection) = 0;
    virtual ~Response() {}
};

}
}
#endif // Response_H
