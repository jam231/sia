#ifndef NETWORKPROTOCOL_UTILITIES_H
#define NETWORKPROTOCOL_UTILITIES_H

#include "utilities.h"


#include "Requests/cancelordermsg.h"
#include "Requests/buystockmsg.h"
#include "Requests/getcompanystatusmsg.h"
#include "Requests/getmyordersmsg.h"
#include "Requests/getmystocksmsg.h"
#include "Requests/getstockinfomsg.h"
#include "Requests/loginusermsg.h"
#include "Requests/registerusermsg.h"
#include "Requests/sellstockmsg.h"
#include "Requests/subscribestockmsg.h"
#include "Requests/unsubscribestockmsg.h"

#include <stdexcept>

namespace NetworkProtocol
{
namespace Requests
{

enum RequestParseStatus
{
    Ok,
    MalformedRequest,
    InvalidRequestType,
    InvalidRequestBody,
    IncompleteRequest
};


class NETWORKPROTOCOLSHARED_EXPORT InvalidRequest : public std::runtime_error
{
public:
    explicit InvalidRequest(const std::string& what_arg)
        : std::runtime_error(what_arg)
    {}
};


// Use when there is not enough bytes in stream to read request
class NETWORKPROTOCOLSHARED_EXPORT IncompleteRequestError : public InvalidRequest
{
public:
    explicit IncompleteRequestError(DTO::Types::Message::MessageLengthType request_length)
        : InvalidRequest(QString("Stream has less than %1 bytes.")
                         .arg(request_length).toStdString())
    {}
};

// Use when there is not enough bytes in stream to read *body* of the request.
class NETWORKPROTOCOLSHARED_EXPORT MalformedRequestError : public InvalidRequest
{
public:
    explicit MalformedRequestError (const std::string& what_arg)
        : InvalidRequest(what_arg)
    {}
};


// Use when type == Message::UNDEFINED_MESSAGE
class NETWORKPROTOCOLSHARED_EXPORT InvalidRequestTypeError : public InvalidRequest
{
public:
    explicit InvalidRequestTypeError()
        : InvalidRequest("Invalid request type.")
    {}
};
// Use when body contents are well formed, but violate data constrains
class NETWORKPROTOCOLSHARED_EXPORT InvalidRequestBodyError : public InvalidRequest
{
public:
    explicit InvalidRequestBodyError (const std::string& what_arg)
        : InvalidRequest(what_arg)
    {}
};




NETWORKPROTOCOLSHARED_EXPORT std::shared_ptr<Request> fromStream(std::shared_ptr<AbstractLogger>,
                                                                 QDataStream&, RequestParseStatus&);
NETWORKPROTOCOLSHARED_EXPORT std::shared_ptr<Request> fromStream(std::shared_ptr<AbstractLogger>,
                                                                 QDataStream&);
NETWORKPROTOCOLSHARED_EXPORT std::shared_ptr<Request> fromStream(QDataStream&);
// Gets request length (IT DOESN'T MODIFY STREAM!) from stream.
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageLengthType getLength(QIODevice*);
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageLengthType getLength(std::shared_ptr<AbstractLogger>,
                                                                              QIODevice*);


// Gets request length and discards sizeof(Message::MessageLengthType) bytes from stream.
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageLengthType readLength(QDataStream&);
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageLengthType readLength(std::shared_ptr<AbstractLogger>,
                                                                               QDataStream&);

// Similar to readLength, but before discarding any bytes from stream it checks
// if there are at least request_length bytes in stream. If not then it throws RequestIncomplete exception.
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageLengthType tryReadLength(
                                         std::shared_ptr<AbstractLogger>, QDataStream&);

NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageLengthType tryReadLength(
                                         std::shared_ptr<AbstractLogger>, QDataStream&,
                                         RequestParseStatus&);


/*
 * Gets request type and discards sizeof(Message::MessageType) bytes from stream.
 * If there isn't enough bytes in the stream it returns MESSAGE_UNDEFINED.
 */
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageType readType_NoEx(QDataStream&);
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageType readType_NoEx(std::shared_ptr<AbstractLogger>,
                                                                            QDataStream&);
// Does the same as readType, but throw InvalidRequestType on either MESSAGE_UNDEFINED
// or not enough bytes.
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageType readType(QDataStream&);
NETWORKPROTOCOLSHARED_EXPORT DTO::Types::Message::MessageType readType(std::shared_ptr<AbstractLogger>,
                                                                       QDataStream&);

}
}
#endif // NETWORKPROTOCOL_UTILITIES_H
