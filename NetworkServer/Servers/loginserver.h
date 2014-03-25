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

#include <../NetworkProtocol/Requests/loginusermsg.h>
#include <../NetworkProtocol/Requests/registerusermsg.h>

/*
 * Established connections and then handles only register and login requests.
 *
 */
class LoginServer : public QThread
{    
    Q_OBJECT
protected:

    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;

    // socket's descriptor is the key
    // BEWARE: http://social.msdn.microsoft.com/Forums/en-US/ab2d4f74-feeb-4677-b655-84650796884c/winsock2-how-unique-are-socket-descriptors?forum=vcgeneral

    QHash<int, Connection* > connections;

    std::unique_ptr<TcpServer> _server;
    QThread* _owningThread;

    int _port;
    std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > _online_users;

    void setupTcpServer();

    void handleRequest(std::shared_ptr<AbstractLogger>, NetworkProtocol::Requests::LoginUser*, int);
    void handleRequest(std::shared_ptr<AbstractLogger>, NetworkProtocol::Requests::RegisterUser*, int);
    void handleRequest(std::shared_ptr<AbstractLogger>, NetworkProtocol::Requests::Request*, int);

public:
    LoginServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                std::shared_ptr<AbstractDataStorageFactory> dataFactory,
                const QHash<QString, QString> &config,
                std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > online_users,
                QThread *owningThread = nullptr);
    ~LoginServer();
protected:
    virtual void run();

public slots:
    void newConnection();
    void removeConnection(int id);
    void processMessageFrom(int id);
signals:
    void newUser(UserConnection*);
};
#endif // LOGINSERVER_H
