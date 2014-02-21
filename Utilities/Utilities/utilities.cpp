#include "utilities.h"


#include <stdexcept>

std::shared_ptr<AbstractLogger> GlobalUtilities::_logger = std::shared_ptr<AbstractLogger>(new DummyLogger());

void GlobalUtilities::setLogger(AbstractLogger* logger)
{
    if(logger == NULL)
        throw std::invalid_argument("GlobalUtilities::setLogger(logger) with logger == NULL.");
    else
        _logger = std::shared_ptr<AbstractLogger>(logger);
}

std::shared_ptr<AbstractLogger> GlobalUtilities::getLogger()
{
    return _logger;
}
