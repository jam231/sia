#ifndef UTILITIES_H
#define UTILITIES_H

#include "logger.h"



#define LOG_INFO(logger, message)       LOG(logger->info, message)
#define LOG_DEBUG(logger, message)      LOG(logger->debug, message)
#define LOG_ERROR(logger, message)      LOG(logger->error, message)
#define LOG_WARNING(logger, message)    LOG(logger->warning, message)
#define LOG_TRACE(logger, message)      LOG(logger->trace, message)

#define GLOBAL_LOG_INFO(message)    LOG_INFO(GlobalUtilities::getLogger(), message)
#define GLOBAL_LOG_DEBUG(message)   LOG_DEBUG(GlobalUtilities::getLogger(), message)
#define GLOBAL_LOG_ERROR(message)   LOG_ERROR(GlobalUtilities::getLogger(), message)
#define GLOBAL_LOG_WARNING(message) LOG_WARNING(GlobalUtilities::getLogger(), message)
#define GLOBAL_LOG_TRACE(message)   LOG_TRACE(GlobalUtilities::getLogger(), message)


/*
 *  Without an IoC container only alternative to this that comes to my mind
 *  is to contain Logger in some kind of singleton.
 *  Below code is some kind of ServiceLocator...
 */
class GlobalUtilities
{
    static std::shared_ptr<AbstractLogger> _logger;
public:
    static void setLogger(std::shared_ptr<AbstractLogger> logger);
    static std::shared_ptr<AbstractLogger> getLogger();
};



#endif // UTILITIES_H
