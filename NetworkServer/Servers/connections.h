#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <QObject>
#include <QTcpSocket>

#include <memory>

#include <../NetworkProtocol/DataTransferObjects/types.h>
#include <../NetworkProtocol/Responses/response.h>

#include <../Utilities/logger.h>

/// STUB
class Connection : public QObject
{
    Q_OBJECT

protected:

    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    QTcpSocket* _socket;

    qint64 _last_message;
    int _id;

    Connection() {}
public:
    void configureConnections() const;
    Connection(std::shared_ptr<AbstractLoggerFactory>,
               QTcpSocket*);

    virtual ~Connection();
    void send(NetworkProtocol::Responses::Response*);
    QTcpSocket* getSocket();
    int getId() const;

protected slots:
    void error(QAbstractSocket::SocketError);
public slots:
    void disconnected();
    void readyRead();
signals:
    void disconnected(int id);
    void readyRead(int id);
};


class UserConnection : public Connection
{
    Q_OBJECT

    NetworkProtocol::DTO::Types::UserIdType _userId;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

public:
    UserConnection(std::shared_ptr<AbstractLoggerFactory>,
                   NetworkProtocol::DTO::Types::UserIdType,
                   QTcpSocket*);

    NetworkProtocol::DTO::Types::UserIdType getUserId() const;

public slots:
    void disconnected();
    void readyRead();

signals:
    void disconnected(NetworkProtocol::DTO::Types::UserIdType);
    void readyRead(NetworkProtocol::DTO::Types::UserIdType);
};

#endif // CONNECTIONS_H
