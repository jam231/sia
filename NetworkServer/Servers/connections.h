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
    std::shared_ptr<QTcpSocket> _socket;

    qint64 _last_message;
    int _id;

    Connection() {}
    void configureConnections() const;
public:
    Connection(std::shared_ptr<AbstractLoggerFactory>,
               std::shared_ptr<QTcpSocket>);

    virtual ~Connection();
/*
    void send(Responses::Response*)
    {

    }
*/
    std::shared_ptr<QTcpSocket> getSocket();
    int getId() const;

public slots:
    void disconnected();
    void readyRead();
signals:
    void disconnected(int id);
    void readyRead(int id);
};







/// STUB
class UserConnection : public Connection
{


    NetworkProtocol::DTO::Types::UserIdType _userId;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

public:
    UserConnection(std::shared_ptr<AbstractLoggerFactory>,
                   NetworkProtocol::DTO::Types::UserIdType,
                   std::shared_ptr<QTcpSocket>);

    NetworkProtocol::DTO::Types::UserIdType getUserId() const;
};

#endif // CONNECTIONS_H
