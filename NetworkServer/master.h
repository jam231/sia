#ifndef MASTER_H
#define MASTER_H

#include <Servers/loginserver.h>

#include <Servers/tradingserver.h>

#include <../NetworkProtocol/DataTransferObjects/types.h>

#include <memory>
#include <utility>

#include <Utils/scheduling.h>
#include <Utils/utils.h>

#include <QHash>

#include <vector>

class MasterServer : public QObject, public QRunnable
{
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;

    QHash<QString, QString> _config;

    std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > _online_users;

    std::vector<std::unique_ptr<TradingServer> > _trading_server_pool;
    // Problems: QObject: Cannot create children for a parent that is in a different thread. etc.
    //QThreadPool _thread_pool;

    std::vector<std::unique_ptr<QThread> > _thread_pool;

    std::unique_ptr<LoginServer> _login_server;

    std::unique_ptr<Balancer<UserConnection*, std::unique_ptr<TradingServer> > > _user_balancer;


protected:
    void setupServers();
public:
    MasterServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                 std::shared_ptr<AbstractDataStorageFactory> dataFactory,
                 const QHash<QString, QString>& config);

    void run();
public slots:

    void distributeUser(std::shared_ptr<UserConnection>);
};



#endif // MASTER_H
