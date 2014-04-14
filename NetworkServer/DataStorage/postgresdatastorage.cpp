#include "postgresdatastorage.h"

#include <cassert>

#include <QSqlError>
#include <QSqlQuery>


using namespace std;

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;




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

    sessions_created.fetchAndAddOrdered(1);
    auto unique_name = QUuid::createUuid().toString();

    unique_ptr<QSqlDatabase> _handle = unique_ptr<QSqlDatabase>(
                    new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL",
                                                               unique_name.toLatin1())));
    _handle->setHostName(_config["host"]);
    _handle->setDatabaseName(_config["name"]);
    _handle->setUserName(_config["username"]);
    _handle->setPassword(_config["password"]);
    _handle->setPort( _config["port"].toInt());

    LOG_DEBUG(logger, QString("Opening postgres connection(%3) to %1:%2.")
                      .arg(_config["host"]).arg(_config["port"])
                      .arg(unique_name));

    if(!_handle->open())
    {
        LOG_ERROR(logger, QString("Error while opening connection to database. "\
                                  "Error: %1").arg(_handle->lastError().text()));
        throw DatastoreError();
    }
    return shared_ptr<AbstractDataSession>(new PostgreDataSession(move(logger),
                                                                  move(_handle)));
}




PostgreDataSession::PostgreDataSession(shared_ptr<AbstractLogger> logger,
                                       unique_ptr<QSqlDatabase> handle)
    : _handle(move(handle))
{
    _logger.swap(logger);

    if(!_handle)
    {
        LOG_ERROR(_logger, "Database handle == nullptr.");
        throw invalid_argument("Database handle cannot be a nullptr.");
    }
}

PostgreDataSession::~PostgreDataSession()
{
    LOG_DEBUG(_logger, QString("Closing connection(%1) to postgreSQL.")
                       .arg(_handle->connectionName()));
    _handle->close();
}

UserIdType PostgreDataSession::registerAccount(const QString& password,
                                               Types::Failure::FailureType* status)
{
    QSqlQuery query(*_handle);
    *status  = Types::Failure::NO_FAILURE;

    query.prepare("SELECT create_user(:password);");
    query.bindValue(":password", password);

    query.setForwardOnly(true);

    //_handle->transaction();
    query.exec();
    //_handle->commit();

    Types::UserIdType userId;

    if(query.first())
    {
        userId = Types::UserIdType(query.value(0).toInt());
    }
    else
    {
        *status = Types::Failure::INVALID_MESSAGE_BODY;
    }
    query.finish();
    return userId;
}

void PostgreDataSession::loginUser(NetworkProtocol::DTO::Types::UserIdType userId,
                                   const QString& password,
                                   Types::Failure::FailureType* status)
{
    QSqlQuery query(*_handle);
    *status  = Types::Failure::NO_FAILURE;

    //query.prepare("SELECT haslo FROM uzytkownik WHERE id_uz = :id;");

    query.setForwardOnly(true);
    query.exec("SELECT password FROM users WHERE user_id="
               % QString::number(userId.value) % ';');

    if(query.first())
    {
        QString pswd = query.value(0).toString();
        if(password != pswd)
        {
            *status = Types::Failure::BAD_USERID_OR_PASSWORD;
        }
    }
    else
    {
        *status = Types::Failure::BAD_USERID_OR_PASSWORD;
    }
    query.finish();
}

OrderIdType PostgreDataSession::sellStock(UserIdType userId, StockIdType stockId,
                                          AmountType amount,PriceType price,
                                          Failure::FailureType* status)
{
    QSqlQuery query(*_handle);
    *status  = Types::Failure::NO_FAILURE;

    //query.prepare("SELECT * FROM zlec_sprzedaz(:userId, :stockId, :amount, :price);");

    query.setForwardOnly(true);

    query.exec("SELECT * FROM zlec_sprzedaz(" % QString::number(userId.value) % ','
               % QString::number(stockId.value) % ',' % QString::number(amount.value)
               % ',' % QString::number(price.value) % ");");

    OrderIdType order_id;

    if(query.first())
    {
        order_id = query.value(0).toInt();
    }
    else
    {
        LOG_DEBUG(_logger, QString("Query not valid: %1")
                           .arg(query.lastError().text()));
        /// TODO: Discern between RESOURCE_NOT_AVAILABLE and INSUFFICENT_FUNDS
        *status = Failure::INSUFFICIENT_FUNDS;
    }

    query.finish();
    return order_id;
}

OrderIdType PostgreDataSession::buyStock(UserIdType userId, StockIdType stockId,
                                         AmountType amount,PriceType price,
                                         Failure::FailureType* status)
{
    QSqlQuery query(*_handle);
    *status  = Types::Failure::NO_FAILURE;


    query.setForwardOnly(true);

    query.exec("SELECT * FROM zlec_kupno(" % QString::number(userId.value) % ','
               % QString::number(stockId.value) % ',' % QString::number(amount.value)
               % ',' % QString::number(price.value) % ");");

    OrderIdType order_id;

    if(query.first())
    {
        order_id = query.value(0).toInt();
    }
    else
    {
        LOG_DEBUG(_logger, QString("Query not valid: %1")
                           .arg(query.lastError().text()));
        /// TODO: Discern between RESOURCE_NOT_AVAILABLE and INSUFFICENT_FUNDS
        *status = Failure::INSUFFICIENT_FUNDS;
    }
    query.finish();
    return order_id;
}
UserStocks PostgreDataSession::getUserStocks(UserIdType userId,
                                             Failure::FailureType* status)
{
    UserStocks user_stocks;

    QSqlQuery query(*_handle);
    *status  = Types::Failure::NO_FAILURE;

    query.setForwardOnly(true);

    query.exec("SELECT stock_id, amount FROM dobra_uz("
               % QString::number(userId.value) % ");");

    while (query.next())
    {
        if(query.value(0).isValid() && query.value(1).isValid())
        {
            user_stocks.addStock(Types::StockIdType(query.value(0).toInt()),
                                 Types::AmountType(query.value(1).toInt()));
        }
        else
        {
            LOG_ERROR(_logger, "Record doesn't have 2 fields.");
        }
    }
    query.finish();
    return user_stocks;
}


vector<shared_ptr<DTO::Order > >
PostgreDataSession::getUserOrders(UserIdType userId,
                                  Failure::FailureType* status)
{
    vector<shared_ptr<DTO::Order > > user_orders;

    QSqlQuery query(*_handle);
    *status  = Types::Failure::NO_FAILURE;

    //query.prepare("SELECT typ, id_zlecenia, id_zasobu, ilosc, limit1 FROM zlecenia_uz(:userId);");


    query.setForwardOnly(true);

    query.exec("SELECT type, order_id, stock_id, amount, limit1 FROM zlecenia_uz("
               % QString::number(userId.value) % ");");

    while (query.next())
    {
        if(query.value(0).isValid() && query.value(1).isValid() &&
                query.value(2).isValid() && query.value(3).isValid())
        {
            // RETURNS TABLE(type integer, order_id integer, stock_id integer,amount integer, limit1 integer)
            auto order_type = Order::toOrderType(query.value(0).toInt());
            auto order_id   = OrderIdType(query.value(1).toInt());
            auto stock_id   = StockIdType(query.value(2).toInt());
            auto amount     = AmountType(query.value(3).toInt());
            auto price      = PriceType(query.value(4).toInt());

            auto order = shared_ptr<DTO::Order>(
                            new DTO::Order(_logger, order_id, order_type,
                                           stock_id, amount, price));

            user_orders.push_back(move(order));
        }
        else
        {
            LOG_ERROR(_logger, "Record doesn't have 4 fields.");
        }
    }
    query.finish();
    return user_orders;
}

void PostgreDataSession::cancelOrder(UserIdType userId, OrderIdType orderId,
                                     Failure::FailureType* status)
{
    QSqlQuery query1(*_handle),
              query2(*_handle);

    *status  = Types::Failure::NO_FAILURE;

    //query1.prepare("DELETE FROM zlecenie_kupna AS zk WHERE zk.id_zlecenia = :orderId AND zk.id_uz = :userId;");

    //query2.prepare("DELETE FROM zlecenie_sprzedazy AS zs WHERE zs.id_zlecenia = :orderId AND zs.id_uz = :userId;");

    query1.setForwardOnly(true);
    query2.setForwardOnly(true);


    _handle->transaction();

    bool query1_valid = query1.exec("DELETE FROM buy_order AS bo WHERE zk.order_id="
                                    % QString::number(orderId.value) % " AND zk.user_id="
                                    % QString::number(userId.value) % ";");
    bool query2_valid = query2.exec("DELETE FROM sell_order AS so WHERE so.order_id="
                                    % QString::number(orderId.value) % " AND so.user_id="
                                    % QString::number(userId.value) % ";");

    // Unfortunately we are unable to check if order was found and deleted or not.
    // We can only check for errors.
    if(!query1_valid || !query2_valid)
    {
        LOG_WARNING(_logger, QString("Query error(s) : %1  and  %2.")
                             .arg(query1.lastError().text())
                             .arg(query2.lastError().text()));
        *status = Types::Failure::RESOURCE_NOT_AVAILABLE;
    }

    query1.finish();
    query2.finish();

    _handle->commit();
}

void PostgreDataSession::startSession(Failure::FailureType* status)
{
    QSqlQuery query(*_handle);
    query.prepare("SELECT rozpocznij_sesje();");
    query.setForwardOnly(true);
    *status  = Types::Failure::NO_FAILURE;

    if(!query.exec())
    {
        LOG_WARNING(_logger, QString("Error while executing query : %1")
                    .arg(query.lastError().text()));
    }
}

void PostgreDataSession::stopSession(Failure::FailureType* status)
{
    QSqlQuery query(*_handle);
    query.prepare("SELECT zakoncz_sesje();");
    query.setForwardOnly(true);
    *status  = Types::Failure::NO_FAILURE;

    if(!query.exec())
    {
        LOG_WARNING(_logger, QString("Error while executing stopSession : %1")
                    .arg(query.lastError().text()));
    }
}


std::vector<NetworkProtocol::DTO::Types::StockIdType> PostgreDataSession::getAvailableStocks()
{
    QSqlQuery query(*_handle);
    query.setForwardOnly(true);

    query.exec("SELECT get_available_stocks();");

    vector<StockIdType> stocks;

    while (query.next())
    {
        if(query.value(0).isValid())
        {
            // RETURNS TABLE(typ integer, id_zasobu integer)
            auto stock_id   = StockIdType(query.value(0).toInt());
            stocks.push_back(stock_id);
        }
        else
        {
            LOG_ERROR(_logger, "Record doesn't have a valid field.");
        }
    }
    query.finish();
    return stocks;
}
