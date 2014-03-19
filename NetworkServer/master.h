#ifndef MASTER_H
#define MASTER_H

#include <Servers/loginserver.h>

#include <Servers/tradingserver.h>

#include <../NetworkProtocol/DataTransferObjects/types.h>

#include <memory>
#include <utility>

#include <Utils/scheduling.h>

#include <QHash>

#include <vector>

class MasterServer : public QObject, public QRunnable
{
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;

    QHash<QString, QString> _config;

    QSet<NetworkProtocol::DTO::Types::UserIdType> _users_online;

    std::vector<std::unique_ptr<TradingServer> > _trading_server_pool;

    std::unique_ptr<Balancer<UserConnection*, std::unique_ptr<TradingServer> > > _user_balancer;

    QThreadPool _thread_pool;
public:
    MasterServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                 std::shared_ptr<AbstractDataStorageFactory> dataFactory,
                 const QHash<QString, QString>& config);

    void run();
public slots:

    void distributeUser(UserConnection*);
};

#endif // MASTER_H
