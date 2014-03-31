#include "master.h"

#include <stdexcept>

#include <cassert>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;

using namespace std;


MasterServer::MasterServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                           shared_ptr<AbstractDataStorageFactory> dataFactory,
                           const QHash<QString, QString> &config)
    : _loggerFactory(loggerFactory), _dataFactory(dataFactory)
{
    moveToThread(this);

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



}
void MasterServer::setupServers()
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_INFO(logger, "Starting Master Server...");

    int trading_servers_count = _config["trading servers"].toInt();

    LOG_INFO(logger, QString("Spinning up %1 trading servers")
                     .arg(trading_servers_count));

    _online_users.reset(new SharedSet<UserIdType>());


    for(int i = 0; i < trading_servers_count; i++)
    {
        auto tradinig_server = shared_ptr<TradingServer>(new TradingServer(_loggerFactory,
                                                                           _dataFactory,
                                                                           _online_users));
        tradinig_server->start();
        tradinig_server->setPriority(QThread::HighPriority);
        _trading_server_pool.push_back(move(tradinig_server));
    }

    auto balancing_strategy = new RoundRobin<shared_ptr<TradingServer>,
                                             vector<shared_ptr<TradingServer> > >(_trading_server_pool);
    _balancing_strategy.reset(balancing_strategy);

    LOG_INFO(logger, "Setting up login server...");

    _login_server.reset(new LoginServer(_loggerFactory, _dataFactory,
                                        _config, _online_users, this));

    _login_server->start();

    connect(_login_server.get(), SIGNAL(newUser(UserConnection*)),
            this,                SLOT(distributeUser(UserConnection*)));

    _login_server->setPriority(QThread::NormalPriority);
}

void MasterServer::distributeUser(UserConnection *user)
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_DEBUG(logger, QString("Distributing user(%1) to a trading server.")
                      .arg(user->getUserId().value));

    assert(_online_users->contains(user->getUserId()));

    auto target = _balancing_strategy->choose();

    assert(user->thread() == this);
    assert(user->getSocket()->thread() == this);
    assert(user->getSocket()->parent() == user);
    assert(user->parent() == 0);

    user->moveToThread(target.get());

    connect(this, SIGNAL(userConnection(UserConnection*)),
            target.get(), SLOT(addUserConnection(UserConnection*)));

    emit userConnection(user);

    disconnect(this, SIGNAL(userConnection(UserConnection*)),
               target.get(), SLOT(addUserConnection(UserConnection*)));
}

void MasterServer::run()
{
    setupServers();
    QThread::run();
}
