#include "eventserver.h"


using namespace std;


EventServer::EventServer(shared_ptr<AbstractDataStorageFactory> dataFactory,
                         shared_ptr<AbstractLoggerFactory> loggerFactory)
    : _dataFactory(move(dataFactory)), _loggerFactory(move(loggerFactory))
{
    auto logger = _loggerFactory->createLoggingSession();

    LOG_TRACE(logger, "Setting up timers for events.");

    // Set timers
}

void EventServer::run()
{
    LOG_INFO(_loggerFactory->createLoggingSession(),
             "Starting new Event Server.");
    _eventLoop.exec();
}

