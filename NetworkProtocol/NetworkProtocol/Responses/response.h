#ifndef RESPONSE_H
#define RESPONSE_H

#include "message.h"

#include <QIODevice>
#include <QDataStream>

namespace NetworkProtocol
{
namespace Responses
{

/*
 *  Abstract class Response
 *
 *  Base for all responses.
 *
 *  Every response has the following format:
 *  <message_length : 2><message_type : 1><body : message_length>
 *
 */
class NETWORKPROTOCOLSHARED_EXPORT Response : public Message
{
protected:
    void sendHeader(QDataStream& out);
public:
    virtual void send(QIODevice* connection) = 0;
    virtual ~Response() {}
};

}
}
#endif // RESPONSE_H
