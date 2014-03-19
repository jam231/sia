#include "datahub.h"

#include <QSqlError>

using namespace std;


const QString PostgreDataHub::ORDER_COMPLETION_CHANNEL = "";
const QString PostgreDataHub::ORDER_CHANGE_CHANNEL = "";
const QString PostgreDataHub::BEST_ORDER_CHANGE_CHANNEL = "";

PostgreDataHub::PostgreDataHub(shared_ptr<AbstractLoggerFactory> loggerFactory,
                               const QHash<QString, QString>& config)
    : _loggerFactory(move(loggerFactory))
{
    if(!_loggerFactory)
    {
        throw invalid_argument("loggerFactory cannot be nullptr.");
    }
    auto logger = _loggerFactory->createLoggingSession();

    if(!config.contains("username"))
    {
        LOG_TRACE(logger, "key: username not found in config.");
        throw invalid_argument("key: username not found in config.");
    }
    if(!config.contains("password"))
    {
        LOG_TRACE(logger, "key: password not found in config.");
        throw invalid_argument("key: password not found in config.");
    }
    if(!config.contains("host"))
    {
        LOG_TRACE(logger, "key: host not found in config.");
        throw invalid_argument("key: host not found in config.");
    }
    if(!config.contains("port"))
    {
        LOG_TRACE(logger, "key: port not found in config.");
        throw invalid_argument("key: port not found in config.");
    }
    if(!config.contains("name"))
    {
        LOG_TRACE(logger, "key: name not found in config.");
        throw invalid_argument("key: name not found in config.");
    }


    _config["username"] = config["username"];
    _config["password"] = config["password"];
    _config["host"] = config["host"];
    _config["port"] = config["port"];

    bool port_to_int;
    _config["port"].toInt(&port_to_int);

    if(!port_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert port to int. config["\
                  "\"port\"] = ").arg(_config["port"]));
        throw invalid_argument("Error while converting port to int.");
    }
    _config["name"] = config["name"];

    LOG_DEBUG(logger, "Establishing connection to database.");

    establishConnection();
}

void PostgreDataHub::establishConnection()
{
    auto logger = _loggerFactory->createLoggingSession();

    _handle = QSqlDatabase::addDatabase("QPSQL",
                                          QLatin1String("SIA notification hub."));

    _handle.setHostName(_config["host"]);
    _handle.setDatabaseName(_config["name"]);
    _handle.setUserName(_config["username"]);
    _handle.setPassword(_config["password"]);
    _handle.setPort( _config["port"].toInt());

    LOG_DEBUG(logger, QString("Opening postgres connection to %1:%2.")
              .arg(_config["host"]).arg(_config["port"]));

    if(!_handle.open())
    {
        LOG_ERROR(logger, QString("Error while opening connection to database. "\
                                  "Error: %1").arg(_handle.lastError().text()));
                throw DatastoreError();
    }

    LOG_DEBUG(logger, "Successfully established connection to database.");

    LOG_DEBUG(logger, "Subscribing to notifications.");

    if(_handle.driver()->subscribeToNotification(ORDER_COMPLETION_CHANNEL))
    {
        LOG_WARNING(logger, QString("Failed to subscribe to channel %1")
                             .arg(ORDER_COMPLETION_CHANNEL));
    }
    if(_handle.driver()->subscribeToNotification(ORDER_CHANGE_CHANNEL))
    {
        LOG_WARNING(logger, QString("Failed to subscribe to channel %1")
                             .arg(ORDER_CHANGE_CHANNEL));
    }
    if(_handle.driver()->subscribeToNotification(BEST_ORDER_CHANGE_CHANNEL))
    {
        LOG_WARNING(logger, QString("Failed to subscribe to channel %1")
                             .arg(BEST_ORDER_CHANGE_CHANNEL));
    }

    LOG_INFO(logger, "Postgre notification hub has been started.");
}

void PostgreDataHub::run()
{
    auto logger = _loggerFactory->createLoggingSession();
    LOG_INFO(logger, "Postgre notification hub is starting...");
    establishConnection();

    _event_loop.exec();
}

void PostgreDataHub::handleOrderCompleted(const QVariant& payload)
{
    /// TODO: implement
}

void PostgreDataHub::handleOrderChange(const QVariant& payload)
{
    /// TODO: implement

}

void PostgreDataHub::handleBestOrderChange(const QVariant& payload)
{
    /// TODO: implement
}

void PostgreDataHub::notificationHandler(const QString& channelName,
                                         QSqlDriver::NotificationSource source,
                                         const QVariant& payload)
{

    if(ORDER_COMPLETION_CHANNEL == channelName)
    {
        handleOrderCompleted(payload);
    }
    else if(ORDER_CHANGE_CHANNEL == channelName)
    {
        handleOrderChange(payload);
    }
    else if(BEST_ORDER_CHANGE_CHANNEL == channelName)
    {
        handleBestOrderChange(payload);
    }
    else
    {
        LOG_DEBUG(_loggerFactory->createLoggingSession(),
                  QString("Message from unhandled channel %1 from %2 with "\
                          "payload = %3 .").arg(channelName).arg(source)
                  .arg(payload.toString()));
    }
}

PostgreDataHub::~PostgreDataHub()
{
    auto logger = _loggerFactory->createLoggingSession();
    LOG_INFO(logger, "Postgre notification hub is closing...");
    _handle.close();
    LOG_INFO(logger, "Postgre notification hub has been closed successufully.");
}
