#include "connections.h"

#include "Utils/utils.h"

#include <../NetworkProtocol/networkprotocol_utilities.h>

#include <QThread>

#include <stdexcept>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;


Connection::Connection(shared_ptr<AbstractLoggerFactory> loggerFactory,
                       shared_ptr<QTcpSocket> socket)
    :   _loggerFactory(loggerFactory)
{
    if(!_loggerFactory)
    {
        throw std::invalid_argument("logger factory cannot be nullptr.");
    }
    _socket = move(socket);
    _id = _socket->socketDescriptor();
    _socket->disconnect();
    configureConnections();
}

void Connection::configureConnections() const
{
    connect(_socket.get(),  SIGNAL(disconnected()),
            this,           SLOT(disconnected()));
    connect(_socket.get(),  SIGNAL(readyRead()),
            this,           SLOT(readyRead()));
}

void Connection::disconnected()
{
    _socket->disconnect();
    emit disconnected(_id);
}

void Connection::readyRead()
{
    auto logger = _loggerFactory->createLoggingSession();
    if(!_socket->isValid())
    {
        LOG_WARNING(logger, QString("Connection with id = %1 has invalid socket: %2. "\
                                    "Commencing socket destruction.")
                            .arg(_id).arg(_socket->errorString()));
        disconnected();
    }
    else
    {
        emit readyRead(_id);
    }
}

int Connection::getId() const
{
    return _id;
}
Connection::~Connection()
{
}


std::shared_ptr<QTcpSocket> Connection::getSocket()
{
    return _socket;
}


UserConnection::UserConnection(shared_ptr<AbstractLoggerFactory> loggerFactory,
                               UserIdType userId, shared_ptr<QTcpSocket> socket)
    : _userId(userId), _loggerFactory(loggerFactory)
{
    _socket = move(socket);
    _id = _socket->socketDescriptor();
    _socket->disconnect();
    configureConnections();
}

UserIdType UserConnection::getUserId() const
{
    return _userId;
}

void UserConnection::disconnected()
{
    _socket->disconnect();
    emit disconnected(_userId);
}

void UserConnection::readyRead()
{
    auto logger = _loggerFactory->createLoggingSession();
    if(!_socket->isValid())
    {
        LOG_WARNING(logger, QString("UserConnection with id = %1 has invalid socket: %2. "\
                                    "Commencing socket destruction.")
                            .arg(_userId.value).arg(_socket->errorString()));
        disconnected();
    }
    else
    {
        emit readyRead(_userId);
    }
}

