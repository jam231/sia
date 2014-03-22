#ifndef DATASTORE_H
#define DATASTORE_H

#include <QSqlDatabase>

#include <memory>

#include <logger.h>

class DatastoreError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Datastore error";
    }
};


#include <../NetworkProtocol/DataTransferObjects/types.h>


class AbstractDataSession
{
protected:
    std::shared_ptr<AbstractLogger>                     _logger;
public:
    void setLogger(std::shared_ptr<AbstractLogger> logger);
    // Abstract commands
    virtual NetworkProtocol::DTO::Types::UserIdType registerAccount(const QString& password,
                                                                    NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;
    virtual void loginUser(NetworkProtocol::DTO::Types::UserIdType, const QString& password,
                           NetworkProtocol::DTO::Types::Failure::FailureType*) = 0;
};

class AbstractDataStorageFactory
{
public:
    virtual std::shared_ptr<AbstractDataSession> openSession() = 0;
};


class PooledDataSession;

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

class PooledDataSession : public AbstractDataSession
{
    std::shared_ptr<AbstractDataSession>        _session;
    std::shared_ptr<PooledDataStorageFactory>   _owner;
public:
    PooledDataSession(std::shared_ptr<AbstractLogger> logger,
                      std::shared_ptr<AbstractDataSession> session,
                      std::shared_ptr<PooledDataStorageFactory> owner);

    NetworkProtocol::DTO::Types::UserIdType registerAccount(const QString& password,
                                                            NetworkProtocol::DTO::Types::Failure::FailureType*);
    void loginUser(NetworkProtocol::DTO::Types::UserIdType, const QString& password,
                           NetworkProtocol::DTO::Types::Failure::FailureType*);

    ~PooledDataSession();
};




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

    NetworkProtocol::DTO::Types::UserIdType registerAccount(const QString& password,
                                                            NetworkProtocol::DTO::Types::Failure::FailureType*);
    void loginUser(NetworkProtocol::DTO::Types::UserIdType, const QString& password,
                           NetworkProtocol::DTO::Types::Failure::FailureType*);

    ~PostgreDataSession();

};









#endif // DATASTORE_H
