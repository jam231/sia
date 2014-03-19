#include "connections.h"

#include "Utils/utils.h"

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;


Connection::Connection(int socket_descriptor)
{
    _socket = shared_ptr<QTcpSocket>(make_qTcpSocket(socket_descriptor));
}

std::shared_ptr<QTcpSocket> Connection::getSocket()
{
    return _socket;
}




UserConnection::UserConnection(UserIdType userId, int socket_descriptor)
    : _userId(userId), Connection(socket_descriptor)
{
}

UserIdType UserConnection::getUserId() const
{
    return _userId;
}
