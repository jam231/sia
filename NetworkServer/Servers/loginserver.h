#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <memory>

#include <QObject>
#include <QRunnable>
#include <QTcpServer>
#include <QEventLoop>

#include "connections.h"

#include <Utils/utils.h>

#include <../Utilities/logger.h>

#include <DataStorage/datastorage.h>

/*
 * Established connections and then handles only register and login requests.
 *
 */
class LoginServer : public QObject, public QRunnable
{    
    Q_OBJECT
protected:

    QEventLoop _event_loop;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;

    QHash<int, std::shared_ptr<Connection> > connections;

    std::unique_ptr<QTcpServer> _server;

    int _port;
    std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > _online_users;


    void setupTcpServer();
public:
    LoginServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                std::shared_ptr<AbstractDataStorageFactory> dataFactory,
                const QHash<QString, QString> &config,
                std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > online_users);
    ~LoginServer();

    void run();

public slots:
    void newConnection();

signals:
    void newUser(std::shared_ptr<UserConnection>);
};
#endif // LOGINSERVER_H
