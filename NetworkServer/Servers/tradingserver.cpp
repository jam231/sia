#include "tradingserver.h"

#include "Utils/utils.h"

#include <stdexcept>


#include <../NetworkProtocol/Responses/failuremsg.h>
#include <../NetworkProtocol/networkprotocol_utilities.h>
#include <../NetworkProtocol/Responses/okmsg.h>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;



/*                     Trading server implementation                    */

TradingServer::TradingServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                             std::shared_ptr<AbstractDataStorageFactory> datastorageFactory,
                             std::shared_ptr<SharedSet<UserIdType> > online_users)
    : _loggerFactory(loggerFactory), _dataStorageFactory(datastorageFactory),
      _online_users(online_users)
{
    // Bradley Hughes, I'm doing it right.
    moveToThread(this);
    if(!_loggerFactory)
    {
        throw invalid_argument("loggerFactory cannot be nullptr.");
    }

    auto logger = _loggerFactory->createLoggingSession();

    if(!_dataStorageFactory)
    {
        LOG_TRACE(logger, "dataStorageFactory cannot be nullptr.");
        throw invalid_argument("dataStorageFactory cannot be nullptr.");
    }
}

void TradingServer::run()
{
    LOG_INFO(_loggerFactory->createLoggingSession(),
             "Starting new Trading Server...");
    QThread::run();
}

/// TODO: Refactor code below
///
void TradingServer::addUserConnection(UserConnection *connection)
{
    auto logger = _loggerFactory->createLoggingSession();
    auto user_connection = shared_ptr<UserConnection>(connection);
    auto user_id = user_connection->getUserId();

    connection->configureConnections();
    if(!_userConnections.contains(user_id))
    {
        // connect() user_connection with this
        _userConnections.insert(user_id, move(user_connection));
        connect(user_connection.get(),  SIGNAL(disconnected(NetworkProtocol::DTO::Types::UserIdType)),
                this,              SLOT(removeConnection(NetworkProtocol::DTO::Types::UserIdType)));
        connect(user_connection.get(), SIGNAL(readyRead(NetworkProtocol::DTO::Types::UserIdType)),
                this,             SLOT(processMessageFrom(NetworkProtocol::DTO::Types::UserIdType)));
        LOG_INFO(logger, QString("Added user with id = %1").arg(user_id.value));
        Responses::Ok confirm_login;
        user_connection->send(&confirm_login);
        //processMessageFrom(user_id);
    }
    else
    {
        LOG_WARNING(logger, "User with id = %1 is already on logged on this"\
                    " trading server. This shouldn't happen.");
        user_connection->disconnected();
    }
}

void TradingServer::processMessageFrom(UserIdType userId)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_TRACE(logger, QString("Reading from user ith id = %1.").arg(userId.value));

    assert( _userConnections.contains(userId));

    auto connection = _userConnections[userId];
    auto socket = connection->getSocket();

    LOG_TRACE(logger, QString("Bytes available: %1")
                      .arg(socket->bytesAvailable()));

    QDataStream stream(socket);
    int requests_per_second = 0;
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    try
    {
        while(_userConnections.contains(userId))
        {
            try
            {
            // Log transaction: request -> data access -> response
            auto logger = _loggerFactory->createLoggingSession();
            auto request = Requests::fromStream(logger, stream);
            switch(request->type())
            {
                case Message::REQUEST_SELL_STOCK_ORDER:
                {
                    //handleRequest(logger, static_cast<Requests::LoginUser*>(request.get()), id);
                }
                break;
                case Message::REQUEST_BUY_STOCK_ORDER:
                {
                    //handleRequest(logger, static_cast<Requests::RegisterUser*>(request.get()), id);
                }
                break;
                default:
                    handleRequest(logger, request.get(), userId);
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
                                    "unknown exception").arg(userId.value));
        auto response = Responses::Failure(Failure::REQUEST_DROPPED);
        response.send(socket);
    }

    auto delay = (QDateTime::currentMSecsSinceEpoch() - timestamp);
    LOG_INFO(logger, QString("Requests per second: %1")
             .arg(requests_per_second * 100 / (delay == 0 ? 1 : delay)));
}

void TradingServer::removeConnection(UserIdType userId)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, QString("Removing user connection with id = %1")
                     .arg(userId.value));
    _userConnections.remove(userId);
    _online_users->remove(userId);
}


void TradingServer::handleRequest(std::shared_ptr<AbstractLogger> logger,
                                  Requests::Request* request, UserIdType userId)
{
    auto source = _userConnections[userId];

    LOG_DEBUG(logger, QString("Sending failure: not authorized to request %1")
             .arg(request->type()));

    auto response = Responses::Failure(Failure::NOT_AUTHORIZED);
    response.send(source->getSocket());
}
