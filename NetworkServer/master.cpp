#include "master.h"

#include <stdexcept>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;


MasterServer::MasterServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                           shared_ptr<AbstractDataStorageFactory> dataFactory,
                           const QHash<QString, QString> &config)
    : _loggerFactory(move(loggerFactory)), _dataFactory(move(dataFactory))
{
    if(!_loggerFactory)
    {
        throw invalid_argument("loggerFactory cannot be nullptr.");
    }
    auto logger = _loggerFactory->createLoggingSession();

    if(!_dataFactory)
    {
        LOG_TRACE(logger, "dataFactory cannot be nullptr.");
        throw invalid_argument("dataFactory cannot be nullptr.");
    }

    if(!config.contains("trading servers"))
    {
        LOG_TRACE(logger, "key: 'trading servers' not found in config.");
        throw invalid_argument("key: 'trading servers' not found in config.");
    }
    _config = config;

    bool trading_servers_to_int;
    int trading_servers_count = _config["trading servers"].toInt(&trading_servers_to_int);

    if(!trading_servers_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert 'trading servers' value to int. config["\
                  "\"trading servers\"] = ").arg(_config["trading servers"]));
        throw invalid_argument("Error while converting 'trading servers' value to int.");
    }

    if(trading_servers_count <= 0)
    {
        LOG_TRACE(logger, "Trading servers count <= 0.");
        throw invalid_argument("Trading servers count <= 0.");
    }

    _online_users.reset(new SharedSet<UserIdType>());

}
void MasterServer::setupServers()
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, "Starting Master Server...");

    int trading_servers_count = _config["trading servers"].toInt();

    LOG_INFO(logger, QString("Setting max thread count of thread pool"\
                     " to %1.").arg(trading_servers_count + 1));

    _thread_pool.setMaxThreadCount(trading_servers_count + 1);

    LOG_INFO(logger, QString("Spining up %1 trading servers").arg(trading_servers_count));

    for(int i = 0; i < trading_servers_count; i++)
    {
        auto server = unique_ptr<TradingServer>(new TradingServer(_loggerFactory,
                                                                  _dataFactory));
        server->setAutoDelete(false);
        _trading_server_pool.push_back(move(server));
        _thread_pool.start(server.get(), QThread::NormalPriority);
    }

    LOG_INFO(logger, "Trading servers are running...");
    LOG_INFO(logger, "Setting up login server...");

    _login_server.reset(new LoginServer(_loggerFactory, _dataFactory,
                                        _config, _online_users));
    _thread_pool.start(_login_server.get());
}

void MasterServer::run()
{
    QEventLoop eventLoop;
    setupServers();
    eventLoop.exec();
}
