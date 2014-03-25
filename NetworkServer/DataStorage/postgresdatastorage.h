#ifndef POSTGRESDATASTORAGE_H
#define POSTGRESDATASTORAGE_H

#include <QSqlDatabase>

#include <DataStorage/datastorage.h>



class PostgresDataStorageFactory : public AbstractDataStorageFactory
{
    QAtomicInt sessions_created;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

    QHash<QString, QString> _config;
public:
    PostgresDataStorageFactory(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                               const QHash<QString, QString>& config);

    std::shared_ptr<AbstractDataSession> openSession();
};


class PostgreDataSession : public AbstractDataSession
{
protected:
    std::unique_ptr<QSqlDatabase>       _handle;
    std::shared_ptr<AbstractLogger>     _logger;
public:
    PostgreDataSession(std::shared_ptr<AbstractLogger> logger,
                       std::unique_ptr<QSqlDatabase> handle);

    NetworkProtocol::DTO::Types::UserIdType
    registerAccount(const QString& password,
                    NetworkProtocol::DTO::Types::Failure::FailureType*);
    void loginUser(NetworkProtocol::DTO::Types::UserIdType, const QString& password,
                   NetworkProtocol::DTO::Types::Failure::FailureType*);

    NetworkProtocol::DTO::Types::OrderIdType
    sellStock(NetworkProtocol::DTO::Types::UserIdType userId,
              NetworkProtocol::DTO::Types::StockIdType stockId,
              NetworkProtocol::DTO::Types::AmountType amount,
              NetworkProtocol::DTO::Types::PriceType price,
              NetworkProtocol::DTO::Types::Failure::FailureType*);

    NetworkProtocol::DTO::Types::OrderIdType
    buyStock(NetworkProtocol::DTO::Types::UserIdType userId,
             NetworkProtocol::DTO::Types::StockIdType stockId,
             NetworkProtocol::DTO::Types::AmountType amount,
             NetworkProtocol::DTO::Types::PriceType price,
             NetworkProtocol::DTO::Types::Failure::FailureType*);

    NetworkProtocol::DTO::UserStocks
    getUserStocks(NetworkProtocol::DTO::Types::UserIdType userId,
                  NetworkProtocol::DTO::Types::Failure::FailureType*);

    std::vector<std::shared_ptr<NetworkProtocol::DTO::Order > >
    getUserOrders(NetworkProtocol::DTO::Types::UserIdType userId,
                  NetworkProtocol::DTO::Types::Failure::FailureType*);

    void cancelOrder(NetworkProtocol::DTO::Types::UserIdType,
                     NetworkProtocol::DTO::Types::OrderIdType,
                     NetworkProtocol::DTO::Types::Failure::FailureType*);

    void startSession(NetworkProtocol::DTO::Types::Failure::FailureType*);

    void stopSession(NetworkProtocol::DTO::Types::Failure::FailureType*);

    ~PostgreDataSession();

};





#endif // POSTGRESDATASTORAGE_H
