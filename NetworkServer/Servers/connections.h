#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <QObject>
#include <QTcpSocket>

#include <memory>

#include <../NetworkProtocol/DataTransferObjects/types.h>

/// STUB
class Connection : public QObject
{
    Q_OBJECT
protected:
    std::shared_ptr<QTcpSocket> _socket;
    int _id;

    Connection() {}
    void configureConnections() const;
public:
    Connection(int socket_descriptor);
    Connection(std::shared_ptr<QTcpSocket>);

    virtual ~Connection();

    std::shared_ptr<QTcpSocket> getSocket();


public slots:
    void disconnected();
signals:
    void disconnected(int id);
};







/// STUB
class UserConnection : public Connection
{
    NetworkProtocol::DTO::Types::UserIdType _userId;

public:
    UserConnection(NetworkProtocol::DTO::Types::UserIdType,
                   std::shared_ptr<QTcpSocket>);

    NetworkProtocol::DTO::Types::UserIdType getUserId() const;
};

#endif // CONNECTIONS_H
