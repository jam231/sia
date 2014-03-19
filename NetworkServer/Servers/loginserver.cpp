#include "loginserver.h"

#include "Utils/utils.h"

#include <cassert>

using namespace std;

LoginServer::LoginServer(std::shared_ptr<AbstractLoggerFactory> logger_factory,
                         const QHash<QString, QString> &config)
    : _loggerFactory(logger_factory)
{

    if(!_loggerFactory)
    {
        throw invalid_argument("logger factory cannot be nullptr.");
    }

    auto logger = _loggerFactory->createLoggingSession();

    if(!config.contains("port"))
    {
        LOG_TRACE(logger, "key: port not found in config.");
        throw invalid_argument("key: port not found in config.");
    }

    bool port_to_int;
    _port = config["port"].toInt(&port_to_int);

    if(!port_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert port to int. config["\
                  "\"port\"] = ").arg(config["port"]));
        throw invalid_argument("Error while converting port to int.");
    }

}

void LoginServer::setupTcpServer()
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, QString("Starting tcp server on port %1.").arg(_port));

    _server = unique_ptr<QTcpServer>(new QTcpServer(this));

    connect(_server.get(), SIGNAL(newConnection()),
            this,     SLOT(addNewConnection()));

    if(!_server->listen(QHostAddress::Any, _port))
    {
        LOG_WARNING(logger, QString("Error occured while establishing tcp server. "\
                    "Error message: %1").arg(_server->errorString()));
        throw TcpConnectionError();
    }
    LOG_INFO(logger, QString("Tcp server listening on port %1.").arg(_port));
}

void LoginServer::newConnection()
{
    auto logger = _loggerFactory->createLoggingSession();

    QTcpSocket* socket = _server->nextPendingConnection();
    LOG_INFO(logger, QString("New connection: %1:%2")
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort()));

    int socket_descriptor = socket->socketDescriptor();

    auto connection = shared_ptr<Connection>(new Connection(socket_descriptor));
    delete socket;

    assert(!connections.contains(connection->getSocket()->socketDescriptor()));

    LOG_TRACE(logger, QString("Inserting new connection to connection pool"\
                              "with key = %1")
                      .arg(connection->getSocket()->socketDescriptor()));
    connections.insert(connection->getSocket()->socketDescriptor(), move(connection));


/* socket_descriptor is connection key.
    connect(newConnection, SIGNAL(disconnected(int)),
              this,        SLOT(disconnect(int)));
*/
}

void LoginServer::run()
{
    LOG_INFO(_loggerFactory->createLoggingSession(),
             "Starting new Login Server.");
   setupTcpServer();

   _event_loop.exec();
}

LoginServer::~LoginServer()
{
    _server->close();
}
