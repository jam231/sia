#ifndef DATASTORE_H
#define DATASTORE_H

#include <../NetworkProtocol/DataTransferObjects/types.h>
#include <../NetworkProtocol/DataTransferObjects/order.h>
#include <../NetworkProtocol/DataTransferObjects/stockinfo.h>
#include <../NetworkProtocol/DataTransferObjects/userstocks.h>


#include <memory>
#include <vector>

#include <logger.h>



class DatastoreError : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Datastore error";
    }
};







class AbstractDataSession
{
protected:
    std::shared_ptr<AbstractLogger>                     _logger;
public:
    void setLogger(std::shared_ptr<AbstractLogger> logger);
    // Abstract commands
    virtual NetworkProtocol::DTO::Types::UserIdType
                 registerAccount(const QString& password,
                                 NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual void loginUser(NetworkProtocol::DTO::Types::UserIdType,
                           const QString& password,
                           NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual NetworkProtocol::DTO::Types::OrderIdType
                 sellStock(NetworkProtocol::DTO::Types::UserIdType userId,
                           NetworkProtocol::DTO::Types::StockIdType stockId,
                           NetworkProtocol::DTO::Types::AmountType amount,
                           NetworkProtocol::DTO::Types::PriceType price,
                           NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual NetworkProtocol::DTO::Types::OrderIdType
                 buyStock(NetworkProtocol::DTO::Types::UserIdType userId,
                          NetworkProtocol::DTO::Types::StockIdType stockId,
                          NetworkProtocol::DTO::Types::AmountType amount,
                          NetworkProtocol::DTO::Types::PriceType price,
                          NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual NetworkProtocol::DTO::UserStocks
                             getUserStocks(NetworkProtocol::DTO::Types::UserIdType userId,
                             NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual std::vector<std::shared_ptr<NetworkProtocol::DTO::Order> >
                             getUserOrders(NetworkProtocol::DTO::Types::UserIdType userId,
                                           NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual void cancelOrder(NetworkProtocol::DTO::Types::UserIdType,
                             NetworkProtocol::DTO::Types::OrderIdType,
                             NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual void startSession(NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual void stopSession(NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;

    virtual std::vector<NetworkProtocol::DTO::Types::StockIdType> getAvailableStocks() = 0;
};



class AbstractDataStorageFactory
{
public:
    virtual std::shared_ptr<AbstractDataSession> openSession() = 0;
};








class PooledDataSession;

/*
 *  Beware, code below:
 *      auto session1 = pooledDataStorageFactory1.openSession();
 *      auto session2 = pooledDataStorageFactory1.openSession();
 *
 *  Can result in deadlock (if pool size == 1).
 *
 *  Releasing of acquired instance of AbstractDataSession is done
 *  automatically via shared_ptr autodelete.
 */
class PooledDataStorageFactory : public AbstractDataStorageFactory
{
private:
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _sessionFactory;

    QQueue<std::shared_ptr<AbstractDataSession> > _pool;

    QMutex _pool_access_lock;
    QWaitCondition _pool_not_empty;

protected:
    void addToPool(std::shared_ptr<AbstractDataSession> session);
public:
    PooledDataStorageFactory(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                                    std::shared_ptr<AbstractDataStorageFactory> sessionFactory,
                                    size_t poolSize);

    std::shared_ptr<AbstractDataSession> openSession();

    friend class PooledDataSession;
};



/*
 *  BEWARE:
 *
 *      If owner is not nullptr, then deleting owner before PooledDataSession
 *      will cause segmentation fault.
 */
class PooledDataSession : public AbstractDataSession
{
    std::shared_ptr<AbstractDataSession>        _session;
    PooledDataStorageFactory*                   _owner;
public:
    PooledDataSession(std::shared_ptr<AbstractLogger> logger,
                      std::shared_ptr<AbstractDataSession> session,
                      PooledDataStorageFactory* owner);

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
         getUserStocks(NetworkProtocol::DTO::Types::UserIdType,
                       NetworkProtocol::DTO::Types::Failure::FailureType*);

    std::vector<std::shared_ptr<NetworkProtocol::DTO::Order > >
         getUserOrders(NetworkProtocol::DTO::Types::UserIdType,
                       NetworkProtocol::DTO::Types::Failure::FailureType*);

    void cancelOrder(NetworkProtocol::DTO::Types::UserIdType,
                      NetworkProtocol::DTO::Types::OrderIdType,
                      NetworkProtocol::DTO::Types::Failure::FailureType*);

    void startSession(NetworkProtocol::DTO::Types::Failure::FailureType*);

    void stopSession(NetworkProtocol::DTO::Types::Failure::FailureType*);

    std::vector<NetworkProtocol::DTO::Types::StockIdType> getAvailableStocks();

    ~PooledDataSession();
};








#endif // DATASTORE_H
