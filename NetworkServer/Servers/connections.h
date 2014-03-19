#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <QObject>
#include <QTcpSocket>

#include <memory>

#include <../NetworkProtocol/DataTransferObjects/types.h>

/// STUB
class Connection : public QObject
{
    std::shared_ptr<QTcpSocket> _socket;

public:
    Connection(int socket_descriptor);

    std::shared_ptr<QTcpSocket> getSocket();
};

/// STUB
class UserConnection : public Connection
{
    NetworkProtocol::DTO::Types::UserIdType _userId;

public:
    UserConnection(NetworkProtocol::DTO::Types::UserIdType userId,
                   int socket_descriptor);

    NetworkProtocol::DTO::Types::UserIdType getUserId() const;
};

#endif // CONNECTIONS_H
