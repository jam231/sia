#include "loginserver.h"

#include "Utils/utils.h"


#include <../NetworkProtocol/Responses/failuremsg.h>
#include <../NetworkProtocol/networkprotocol_utilities.h>
#include <../NetworkProtocol/Responses/registerusersuccessmsg.h>
#include <../NetworkProtocol/Responses/okmsg.h>

#include <cassert>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;
using namespace Message;


using namespace std;


LoginServer::LoginServer(shared_ptr<AbstractLoggerFactory> loggerFactory,
                         shared_ptr<AbstractDataStorageFactory> dataFactory,
                         const QHash<QString, QString> &config,
                         shared_ptr<SharedSet<UserIdType> > online_users,
                         QThread* owningThread)
    : _loggerFactory(loggerFactory),  _dataFactory(dataFactory),
      _online_users(move(online_users)), _owningThread(owningThread)
{
    if(_owningThread == nullptr)
    {
        _owningThread = this;
    }
    // Bradley Hughes, I'm doing it right.
    moveToThread(this);
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

    auto socket = _server->nextPendingConnection();
    //assert(this->thread() == socket->thread());
    LOG_INFO(logger, QString("New connection: %1:%2")
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort()));
    int socket_descriptor = socket->socketDescriptor();
    assert(!connections.contains(socket_descriptor));
    if(!connections.contains(socket_descriptor))
    {
        auto connection = shared_ptr<Connection>(new Connection(_loggerFactory, socket));
        /*
         *  http://stackoverflow.com/questions/10711246/move-to-thread-causes-issue
         *  socket->moveToThread(connection->thread());
         *
         */
        LOG_TRACE(logger, QString("Inserting new connection to connection pool"\
                              "with key = %1")
                          .arg(connection->getSocket()->socketDescriptor()));
        connections.insert(connection->getSocket()->socketDescriptor(), connection);

        LOG_INFO(logger, QString("Active connections: %1").arg(connections.size()));


        connect(connection.get(),  SIGNAL(disconnected(int)),
                this,              SLOT(removeConnection(int)));
        connect(connection.get(), SIGNAL(readyRead(int)),
                this,             SLOT(processMessageFrom(int)));
        //processMessageFrom(socket_descriptor);
    }
}

void LoginServer::processMessageFrom(int id)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_TRACE(logger, QString("Reading from socket with id = %1.").arg(id));

    auto connection = connections[id];
    auto socket = connection->getSocket();

    LOG_TRACE(logger, QString("Bytes available: %1").arg(socket->bytesAvailable()));

    QDataStream stream(socket);
    int requests_per_second = 0;
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    try
    {
        // Previous request might moved current connection out, e.g. to trading server.
        while(connections.contains(id))
        {
            try
            {
            // Log transaction: request -> data access -> response
            auto logger = _loggerFactory->createLoggingSession();
            auto request = Requests::fromStream(logger, stream);
            switch(request->type())
            {
                case Message::REQUEST_LOGIN_USER:
                {
                    handleRequest(logger, static_cast<Requests::LoginUser*>(request.get()), id);
                }
                break;
                case Message::REQUEST_REGISTER_USER:
                {
                    handleRequest(logger, static_cast<Requests::RegisterUser*>(request.get()), id);
                }
                break;
                default:
                    handleRequest(logger, request.get(), id);
                break;
            }
                requests_per_second++;
            }
            catch(Requests::MalformedRequest& e)
            {
                LOG_TRACE(logger, QString("Malformed request: %1").arg(e.what()));
                auto response = Responses::Failure(Failure::MALFORMED_MESSAGE);
                response.send(socket);
            }
            catch(Requests::InvalidRequestType& e)
            {
                LOG_TRACE(logger, QString("Invalid request type: %1").arg(e.what()));
                auto response = Responses::Failure(Failure::UNRECOGNIZED_MESSAGE);
                response.send(socket);
            }
            catch(Requests::InvalidRequestBody& e)
            {
                LOG_TRACE(logger, QString("Invalid request body: %1").arg(e.what()));
                auto response = Responses::Failure(Failure::INVALID_MESSAGE_BODY);
                response.send(socket);
            }
            catch(DatastoreError& e)
            {
                LOG_WARNING(logger, QString("Database error %1").arg(e.what()));
                auto response = Responses::Failure(Failure::REQUEST_DROPPED);
                response.send(socket);
            }
        }
    }
    catch(Requests::IncompleteRequest& e)
    {
        LOG_TRACE(logger, QString("Request not yet ready: %1").arg(e.what()));
    }
    catch(...)
    {
        LOG_ERROR(logger, QString("Connection with id = %1 has thrown "\
                                    "unknown exception").arg(id));
        auto response = Responses::Failure(Failure::REQUEST_DROPPED);
        response.send(socket);
    }

    auto delay = (QDateTime::currentMSecsSinceEpoch() - timestamp);
    LOG_INFO(logger, QString("Requests per second: %1")
             .arg(requests_per_second * 100 / (delay == 0 ? 1 : delay)));
}



void LoginServer::handleRequest(std::shared_ptr<AbstractLogger> logger,
                                Requests::Request* request, int id)
{
    auto source = connections[id];

    LOG_DEBUG(logger, QString("Sending failure: not authorized to request %1")
             .arg(request->type()));

    auto response = Responses::Failure(Failure::NOT_AUTHORIZED);
    response.send(source->getSocket());
}

void LoginServer::handleRequest(std::shared_ptr<AbstractLogger> logger,
                                Requests::RegisterUser* request, int id)
{
    auto source = connections[id];

    LOG_DEBUG(logger, QString("Connection with id = %1 issues register request.")
              .arg(source->getId()));

    auto session = _dataFactory->openSession();
    Failure::FailureType status = Failure::NO_FAILURE;
    auto user_id = session->registerAccount(request->getPassword(), &status);
    if(status != Failure::NO_FAILURE)
    {
        auto response = Responses::Failure(status);
        response.send(source->getSocket());
    }
    else
    {
        auto response = Responses::RegisterUserSuccess(logger, user_id);
        response.send(source->getSocket());
    }
}


void LoginServer::handleRequest(std::shared_ptr<AbstractLogger> logger,
                                Requests::LoginUser* request, int id)
{
    auto source = connections[id];
    LOG_DEBUG(logger, QString("Connection with id = %1 issues login request "\
                              "for user id = %2.")
              .arg(source->getId()).arg(request->getUserId().value));

    auto user_id = request->getUserId();
    // // Check if password and id match
    // auto session = _dataFactory->openSession()
    // Types::FailureType status;
    // session->loginUser(user_id, request->getPassword(), &status);
    // if(status == Failure::NO_FAILURE)
    // {
    //      //
    //
    // }
    // else
    // {
    //   auto response = Responses::Failure(Failure::BAD_USERID_OR_PASSWORD);
    //   response.send(source->getSocket().get());
    // }
    bool added = _online_users->add(user_id);
    if(!added)
    {
        LOG_TRACE(logger, QString("User with id = %1 is alread online.")
                  .arg(user_id.value));
        auto response = Responses::Failure(Failure::ALREADY_LOGGED);
        response.send(source->getSocket());
    }
    else
    {
        LOG_TRACE(logger, QString("User id = %1 added.").arg(user_id.value));

        // Should moveToThread socket
        // Also remember that moveToThread should be called from thread
        // you move object from (! coupling alert !)
        //source->getSocket()->setParent(0);
        //source->getSocket()->moveToThread(_owningThread);
        //LOG_TRACE(logger, QString("Threads match %1")
        //          .arg(source->getSocket()->thread() == this));
        auto user = new UserConnection(_loggerFactory, user_id,
                                       source->getSocket());
        user->getSocket()->disconnect();
        user->disconnect();
        assert(user->thread() == this);
        assert(user->getSocket()->thread() == this);
        assert(user->getSocket()->parent() == user);
        assert(user->parent() == 0);
        user->moveToThread(_owningThread);
        //user->getSocket()->moveToThread(_owningThread);

        connections.remove(id);

        emit newUser(user);
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
    LOG_INFO(_loggerFactory->createLoggingSession(), "Starting new Login Server.");
    setupTcpServer();
    exec();
}

LoginServer::~LoginServer()
{
    _server->close();
}
