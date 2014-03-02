#ifndef REQUEST_H
#define REQUEST_H

#include "message.h"

#include <QIODevice>
#include <QDataStream>

#include <memory>

namespace NetworkProtocol
{
namespace Requests
{
/// TODO:   Add description with serialization format.

/*
 *  Abstract class Request
 *
 *  Base for all requests.
 *
 *  Every request has the following format:
 *  <message_length : 2><message_type : 1><body : message_length>
 *
 */

class NETWORKPROTOCOLSHARED_EXPORT Request : public Message
{
protected:
    //Request(QDataStream& in);
    Request();
    //void validateRequest(QDataStream& in);
public:
    virtual ~Request() {}
};



}
}
#endif // REQUEST_H
