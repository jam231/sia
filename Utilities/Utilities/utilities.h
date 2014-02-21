#ifndef UTILITIES_H
#define UTILITIES_H

#include "logger.h"



#define LOG_INFO(message) LOG(GlobalUtilities::getLogger()->info, message)
#define LOG_DEBUG(message) LOG(GlobalUtilities::getLogger()->debug, message)
#define LOG_ERROR(message) LOG(GlobalUtilities::getLogger()->error, message)
#define LOG_WARNING(message) LOG(GlobalUtilities::getLogger()->warning, message)
#define LOG_TRACE(message) LOG(GlobalUtilities::getLogger()->trace, message)



/*
 *  Without an IoC container only alternative to this that comes to my mind
 *  is to contain Logger in some kind of singleton.
 *  Below code is some kind of ServiceLocator...
 */
class GlobalUtilities
{
    static std::shared_ptr<AbstractLogger> _logger;
public:


    static void setLogger(AbstractLogger* logger);
    static std::shared_ptr<AbstractLogger> getLogger();
};



#endif // UTILITIES_H
