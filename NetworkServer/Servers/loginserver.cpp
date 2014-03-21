#include "loginserver.h"

#include "Utils/utils.h"

#include <cassert>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;
using namespace std;


LoginServer::LoginServer(shared_ptr<AbstractLoggerFactory> loggerFactory,
                         shared_ptr<AbstractDataStorageFactory> dataFactory,
                         const QHash<QString, QString> &config,
                         shared_ptr<SharedSet<UserIdType> > online_users)
    : _loggerFactory(loggerFactory),  _dataFactory(dataFactory),
      _online_users(move(online_users))
{

    if(!_loggerFactory)
    {
        throw invalid_argument("logger factory cannot be nullptr.");
    }

    auto logger = _loggerFactory->createLoggingSession();

    if(!_dataFactory)
    {
        LOG_TRACE(logger, "data factory cannot be nullptr.");
        throw invalid_argument("data factory cannot be nullptr.");
    }

    if(!_online_users)
    {
        LOG_TRACE(logger, "online users storage cannot be nullptr.");
        throw invalid_argument("online users storage cannot be nullptr.");
    }

    if(!config.contains("server port"))
    {
        LOG_TRACE(logger, "key: 'server port' not found in config.");
        throw invalid_argument("key: 'server port' not found in config.");
    }

    bool port_to_int;
    _port = config["server port"].toInt(&port_to_int);

    if(!port_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert port to int. config["\
                  "\"server port\"] = ").arg(config["server port"]));
        throw invalid_argument("Error while converting port to int.");
    }

}

void LoginServer::setupTcpServer()
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, QString("Starting tcp server on port %1.").arg(_port));

    _server = unique_ptr<TcpServer>(new TcpServer());

    connect(_server.get(), SIGNAL(newConnection()),
            this,     SLOT(newConnection()));

    if(!_server->listen(QHostAddress::Any, _port))
    {
        LOG_WARNING(logger, QString("Error occured while establishing tcp server. "\
                    "Error message: %1").arg(_server->errorString()));
        throw TcpConnectionError();
    }
    LOG_INFO(logger, QString("Tcp server listening on port %1.").arg(_port));
}

// http://qt-project.org/doc/note_revisions/33/54/view

void LoginServer::newConnection()
{
    auto logger = _loggerFactory->createLoggingSession();

    auto socket = shared_ptr<QTcpSocket>(_server->nextPendingConnection());

    LOG_INFO(logger, QString("New connection: %1:%2")
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort()));
    //socket->setParent(this);
    int socket_descriptor = socket->socketDescriptor();
    assert(!connections.contains(socket_descriptor));
    if(!connections.contains(socket_descriptor))
    {
        auto connection = shared_ptr<Connection>(new Connection(socket));
        /*
         *  http://stackoverflow.com/questions/10711246/move-to-thread-causes-issue
         *  socket->moveToThread(connection->thread());
         *
         */
        LOG_TRACE(logger, QString("Inserting new connection to connection pool"\
                              "with key = %1")
                          .arg(connection->getSocket()->socketDescriptor()));
        connections.insert(connection->getSocket()->socketDescriptor(), move(connection));


        connect(connection.get(),  SIGNAL(disconnected(int)),
                this,              SLOT(removeConnection(int)));

    }
}

void LoginServer::removeConnection(int id)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, QString("Removing connection with id = %1").arg(id));
    connections.remove(id);
}

void LoginServer::run()
{

    //this->moveToThread(QThread::currentThread());
    QEventLoop event_loop;
    LOG_INFO(_loggerFactory->createLoggingSession(), "Starting new Login Server.");
    setupTcpServer();

    event_loop.exec();
}

LoginServer::~LoginServer()
{
    _server->close();
}
