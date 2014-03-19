#include "tradingserver.h"

#include "Utils/utils.h"

#include <stdexcept>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;


/*                     Trading server implementation                    */

TradingServer::TradingServer(std::shared_ptr<AbstractLoggerFactory> logger_factory,
                             std::shared_ptr<AbstractDataStorageFactory> datastorage_factory)
    : _loggerFactory(logger_factory), _dataStorageFactory(datastorage_factory)
{

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
    _event_loop.exec();
}

/// TODO: Refactor code below
///
void TradingServer::addUserConnection(std::shared_ptr<UserConnection> connection)
{
    auto logger = _loggerFactory->createLoggingSession();
    auto user_id = connection->getUserId();

    if(!_userConnections.contains(user_id))
    {
        auto user_connection = std::shared_ptr<UserConnection>(
                    // Copy object - connection probably came across thread so its
                    // state is invalid.
                    new UserConnection(user_id, connection->getSocket()->socketDescriptor()));
        // connect() user_connection with this
        _userConnections.insert(user_id, move(user_connection));

        LOG_INFO(logger, QString("Added user with id = %1").arg(user_id.value));

    }
    else
    {
        LOG_WARNING(logger, "User with id = %1 is already on logged on this"\
                    " trading server. This shouldn't happen.");
    }
}
