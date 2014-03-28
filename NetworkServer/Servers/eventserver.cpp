#include "eventserver.h"
#include <../NetworkProtocol/DataTransferObjects/types.h>


using namespace NetworkProtocol::DTO::Types;
using namespace std;


EventServer::EventServer(shared_ptr<AbstractLoggerFactory> loggerFactory,
                         shared_ptr<AbstractDataStorageFactory> dataStorageFactory,
                         const QHash<QString, QString>& config)
    : _dataFactory(move(dataStorageFactory)), _loggerFactory(move(loggerFactory))
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

    bool session_on_to_int;
    int session_on_interval = config["session on interval"].toInt(&session_on_to_int);

    if(!session_on_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert 'session on interval' value to int. config["\
                  "\"session on interval\"] = ").arg(config["session on interval"]));
        throw invalid_argument("Error while converting 'session on interval' value to int.");
    }

    _session_on_interval_ms = session_on_interval * 1000;
    if(_session_on_interval_ms <= 0)
    {
        LOG_TRACE(logger, "Session on interval <= 0.");
        throw invalid_argument("Session on interval <= 0.");
    }

    bool session_off_to_int;
    int session_off_interval = config["session off interval"].toInt(&session_off_to_int);

    if(!session_on_to_int)
    {
        LOG_TRACE(logger, QString("Cannot convert 'session off interval' value to int. config["\
                  "\"session off interval\"] = ").arg(config["session off interval"]));
        throw invalid_argument("Error while converting 'session off interval' value to int.");
    }

    _session_off_interval_ms = session_off_interval * 1000;

    if(_session_off_interval_ms <= 0)
    {
        LOG_TRACE(logger, "Session on interval <= 0.");
        throw invalid_argument("Session on interval <= 0.");
    }
}

void EventServer::run()
{
    auto logger = _loggerFactory->createLoggingSession();
    LOG_INFO(logger, "Starting new Event Server.");

    LOG_TRACE(logger, "Setting up timers for events.");

    // Set timers
    _sessionOffTimer.reset(new QTimer(this));
    _sessionOffTimer->setInterval(_session_off_interval_ms);

    _sessionOnTimer.reset(new QTimer(this));
    _sessionOnTimer->setInterval(_session_on_interval_ms);

    connect(_sessionOnTimer.get(), SIGNAL(timeout()),
            this,             SLOT(sessionOff()));

    connect(_sessionOffTimer.get(), SIGNAL(timeout()),
            this,              SLOT(sessionOn()));

    sessionOn();
    exec();
}

void EventServer::sessionOn()
{
    auto logger = _loggerFactory->createLoggingSession();
    LOG_INFO(logger, "Session on");

    _sessionOffTimer->stop();

    auto data_session = _dataFactory->openSession();

    Failure::FailureType status;
    data_session->startSession(&status);

    _sessionOnTimer->start();
    if(status != Failure::NO_FAILURE)
    {
        LOG_WARNING(logger, QString("data session returned: %1").arg(status));
    }
}

void EventServer::sessionOff()
{
    auto logger = _loggerFactory->createLoggingSession();
    LOG_INFO(logger, "Session off");

    _sessionOnTimer->stop();

    auto data_session = _dataFactory->openSession();

    Failure::FailureType status;
    data_session->stopSession(&status);

    _sessionOffTimer->start();
    if(status != Failure::NO_FAILURE)
    {
        LOG_WARNING(logger, QString("data session returned: %1").arg(status));
    }
}
