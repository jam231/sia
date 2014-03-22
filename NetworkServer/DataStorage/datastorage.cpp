#include "datastorage.h"

#include <cassert>

#include <QSqlError>
#include <QSqlQuery>


using namespace std;

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;



void AbstractDataSession::setLogger(shared_ptr<AbstractLogger> logger)
{
    if(!logger)
    {
        throw invalid_argument("logger cannot be a nullptr.");
    }
    _logger.swap(logger);
}

PostgresDataStorageFactory::PostgresDataStorageFactory(shared_ptr<AbstractLoggerFactory> loggerFactory,
                                                       const QHash<QString, QString> &config)
    : _loggerFactory(move(loggerFactory))
{
    if(!_loggerFactory)
    {
        throw invalid_argument("loggerFactory cannot be nullptr.");
    }
    auto logger = _loggerFactory->createLoggingSession();

    if(!config.contains("database username"))
    {
        LOG_TRACE(logger, "key: 'database username' not found in config.");
        throw invalid_argument("key: 'database username' not found in config.");
    }
    if(!config.contains("database password"))
    {
        LOG_TRACE(logger, "key: 'database password' not found in config.");
        throw invalid_argument("key: 'database password' not found in config.");
    }
    if(!config.contains("database host"))
    {
        LOG_TRACE(logger, "key: 'database host' not found in config.");
        throw invalid_argument("key: 'database host' not found in config.");
    }
    if(!config.contains("database port"))
    {
        LOG_TRACE(logger, "key: 'database port' not found in config.");
        throw invalid_argument("key: 'database port' not found in config.");
    }
    if(!config.contains("database name"))
    {
        LOG_TRACE(logger, "key: 'database name' not found in config.");
        throw invalid_argument("key: 'database name' not found in config.");
    }


    _config["username"] = config["database username"];
    _config["password"] = config["database password"];
    _config["host"] = config["database host"];
    _config["port"] = config["database port"];

    bool port_to_int;
    _config["port"].toInt(&port_to_int);

    if(!port_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert port to int. config["\
                  "\"port\"] = ").arg(_config["port"]));
        throw invalid_argument("Error while converting port to int.");
    }
    _config["name"] = config["database name"];
}

std::shared_ptr<AbstractDataSession> PostgresDataStorageFactory::openSession()
{
    auto logger = _loggerFactory->createLoggingSession();

    int suffix = sessions_created.fetchAndAddOrdered(1);
    unique_ptr<QSqlDatabase> _handle = unique_ptr<QSqlDatabase>(new QSqlDatabase());
    _handle->addDatabase("QPSQL", QString("Rynki finansowe %1")
                                  .arg(suffix).toLatin1());

    _handle->setHostName(_config["host"]);
    _handle->setDatabaseName(_config["name"]);
    _handle->setUserName(_config["username"]);
    _handle->setPassword(_config["password"]);
    _handle->setPort( _config["port"].toInt());

    LOG_DEBUG(logger, QString("Opening postgres connection to %1:%2.")
              .arg(_config["host"]).arg(_config["port"]));

    if(!_handle->open())
    {
        LOG_ERROR(logger, QString("Error while opening connection to database. "\
                  "Error: %1").arg(_handle->lastError().text()));
                throw DatastoreError();
    }
    return shared_ptr<AbstractDataSession>(new PostgreDataSession(move(logger),
                                                                  move(_handle)));
}





PooledDataStorageFactory::PooledDataStorageFactory(shared_ptr<AbstractLoggerFactory> loggerFactory,
                                                   shared_ptr<AbstractDataStorageFactory> sessionFactory,
                                                   size_t poolSize)
    : _loggerFactory(move(loggerFactory)), _sessionFactory(move(sessionFactory))
{
    if(!_loggerFactory)
    {
        throw invalid_argument("loggerFactory cannot be a nullptr.");
    }

    auto logger = _loggerFactory->createLoggingSession();

    if(!_sessionFactory)
    {
        LOG_DEBUG(logger, "sessionFactory == nullptr.");
        throw invalid_argument("sessionFactory cannot be a nullptr.");
    }

    if(poolSize == 0)
    {
        LOG_DEBUG(logger, "pool size == 0");
        throw invalid_argument("pool size must be at least 1.");
    }
    assert(_pool.empty());

    LOG_TRACE(logger, QString("Generating %1 postgre sessions.").arg(poolSize));

    _pool_access_lock.lock();
    for(size_t i = 0; i < poolSize; i++)
    {
        _pool.append(move(_sessionFactory->openSession()));
    }
    _pool_access_lock.unlock();
    assert(_pool.size() == poolSize);

    LOG_TRACE(logger, QString("Created pooled postgres data storage with "\
                              "%1 sessions.").arg(_pool.size()));
}

void PooledDataStorageFactory::addToPool(shared_ptr<AbstractDataSession> session)
{
    _pool_access_lock.lock();

    _pool.enqueue(move(session));
    _pool_not_empty.wakeOne();

    _pool_access_lock.unlock();
}

shared_ptr<AbstractDataSession> PooledDataStorageFactory::openSession()
{
    _pool_access_lock.lock();

    _pool_not_empty.wait(&_pool_access_lock);
    auto session = move(_pool.dequeue());

    _pool_access_lock.unlock();

    auto logger = _loggerFactory->createLoggingSession();
    // That is sooo wrong. Well... yolo!
    auto owner = shared_ptr<PooledDataStorageFactory>(this);
    auto pooled_session = shared_ptr<AbstractDataSession>(
                new PooledDataSession(move(logger), move(session), move(owner)));

    return move(pooled_session);
}









/*                              SESSIONS                            */

PostgreDataSession::PostgreDataSession(shared_ptr<AbstractLogger> logger,
                                       unique_ptr<QSqlDatabase> handle)
    : _handle(move(handle))
{
    setLogger(move(logger));

    if(!_handle)
    {
        LOG_DEBUG(_logger, "Database handle == nullptr.");
        throw invalid_argument("Database handle cannot be a nullptr.");
    }
}

PostgreDataSession::~PostgreDataSession()
{
    _handle->close();
}

UserIdType PostgreDataSession::registerAccount(const QString& password,
                                               Types::Failure::FailureType* failed)
{
    QSqlQuery query(*_handle);

    //*failed  = Types::Failure::NO_FAILURE;
    //
    query.prepare("SELECT nowy_uzytkownik(:password);");
    query.bindValue(":password", password);
    //
    query.setForwardOnly(true);

    _handle->transaction();
    query.exec();
    _handle->commit();

    Types::UserIdType userId;

    if(query.first())
    {
        userId = Types::UserIdType(query.value(0).toInt());
    }
    else
    {
        *failed = Types::Failure::INVALID_MESSAGE_BODY;
    }
    return userId;
}

void PostgreDataSession::loginUser(NetworkProtocol::DTO::Types::UserIdType userId,
                                   const QString& password,
                                   Types::Failure::FailureType* failed)
{
    QSqlQuery query(*_handle);
    *failed  = Types::Failure::NO_FAILURE;
    query.prepare("SELECT haslo FROM uzytkownik WHERE id_uz = :id ;");
    query.bindValue(":id", userId.value);

    query.setForwardOnly(true);

    _handle->transaction();
    query.exec();
    _handle->commit();

    //QString failReason;
    if(query.first())
    {
        QString pswd = query.value(0).toString();
        if(password != pswd)
        {
            *failed = Types::Failure::BAD_USERID_OR_PASSWORD;
        }
    }
    else
    {
        *failed = Types::Failure::BAD_USERID_OR_PASSWORD;
    }
}

PooledDataSession::PooledDataSession(shared_ptr<AbstractLogger> logger,
                                     shared_ptr<AbstractDataSession> session,
                                     std::shared_ptr<PooledDataStorageFactory> owner)
    : _session(move(session)), _owner(owner)
{
    setLogger(logger);

    if(!_session)
    {
        LOG_DEBUG(logger, "session == nullptr");
        throw invalid_argument("session cannot be nullptr");
    }
    session->setLogger(move(logger));
}

void PooledDataSession::loginUser(UserIdType userId, const QString& password,
                                  Failure::FailureType* fail)
{
    _session->loginUser(userId, password, fail);
}

UserIdType PooledDataSession::registerAccount(const QString &password,
                                              Failure::FailureType *fail)
{
    return _session->registerAccount(password, fail);
}

PooledDataSession::~PooledDataSession()
{
    if(_owner)
    {
        LOG_TRACE(_logger, "session object being returned to session pool.");
        _owner->addToPool(move(_session));
    }
}
