#ifndef Request_H
#define Request_H

#include "message.h"

#include <QIODevice>
#include <QDataStream>

#include <exception>

namespace NetworkProtocol
{
namespace Requests
{

class NETWORKPROTOCOLSHARED_EXPORT InvalidRequest : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Invalid data in request.";
    }
};


class NETWORKPROTOCOLSHARED_EXPORT Request : public Message
{
protected:
    Request(QDataStream& in);
    Request();
    void validateRequest(QDataStream& in);
public:
    static DTO::Types::MessageType getType(QDataStream &in);
    static DTO::Types::MessageLengthType getMessageLength(QIODevice* data);

    static const QString getMessageName();

    virtual ~Request() {}
};
}
}
#endif // Request_H
