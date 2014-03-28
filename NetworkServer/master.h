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

class MasterServer : public QThread
{
    Q_OBJECT

    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;

    QHash<QString, QString> _config;

    std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > _online_users;

    std::vector<std::shared_ptr<TradingServer> > _trading_server_pool;
    // Problems: QObject: Cannot create children for a parent that is in a different thread. etc.
    //QThreadPool _thread_pool;

    std::shared_ptr<LoginServer> _login_server;

    std::shared_ptr<BalancingStrategy<std::shared_ptr<TradingServer>,
                                      std::vector<std::shared_ptr<TradingServer> > > > _balancing_strategy;


protected:
    void setupServers();
    virtual void run();

public:
    MasterServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                 std::shared_ptr<AbstractDataStorageFactory> dataFactory,
                 const QHash<QString, QString>& config);

public slots:
    void distributeUser(UserConnection*);

signals:
    void userConnection(UserConnection*);
};



#endif // MASTER_H
