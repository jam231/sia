#include "connections.h"

#include "Utils/utils.h"

#include <QThread>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;


Connection::Connection(int socket_descriptor)
{
    _id = socket_descriptor;
    _socket = shared_ptr<QTcpSocket>(make_qTcpSocket(socket_descriptor));
    configureConnections();
}
Connection::Connection(shared_ptr<QTcpSocket> socket)
{
    _socket = move(socket);
    _id = _socket->socketDescriptor();
    _socket->disconnect();
    configureConnections();
}

void Connection::configureConnections() const
{
    connect(_socket.get(),  SIGNAL(disconnected()),
            this,           SLOT(disconnected()));

}

void Connection::disconnected()
{
    _socket->disconnect();
    emit disconnected(_id);
}

Connection::~Connection()
{
}


std::shared_ptr<QTcpSocket> Connection::getSocket()
{
    return _socket;
}


UserConnection::UserConnection(UserIdType userId, shared_ptr<QTcpSocket> socket)
    : _userId(userId)
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

