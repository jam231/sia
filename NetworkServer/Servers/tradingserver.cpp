#include "tradingserver.h"

#include "Utils/utils.h"

#include <stdexcept>

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
void TradingServer::addUserConnection(std::shared_ptr<UserConnection> connection)
{
    auto logger = _loggerFactory->createLoggingSession();
    auto user_id = connection->getUserId();

    if(!_userConnections.contains(user_id))
    {
        // connect() user_connection with this
        _userConnections.insert(user_id, move(connection));
        connect(connection.get(),  SIGNAL(disconnected(UserIdType)),
                this,              SLOT(removeConnection(UserIdType)));
        connect(connection.get(), SIGNAL(readyRead(UserIdType)),
                this,             SLOT(processMessageFrom(UserIdType)));
        LOG_INFO(logger, QString("Added user with id = %1").arg(user_id.value));
        processMessageFrom(user_id);
    }
    else
    {
        LOG_WARNING(logger, "User with id = %1 is already on logged on this"\
                    " trading server. This shouldn't happen.");
        connection->disconnected();
    }
}

void TradingServer::processMessageFrom(UserIdType userId)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_TRACE(logger, QString("Reading from user ith id = %1.").arg(userId.value));
}

void TradingServer::removeConnection(UserIdType userId)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, QString("Removing user connection with id = %1").arg(userId.value));
    _userConnections.remove(userId);
    _online_users->remove(userId);
}
