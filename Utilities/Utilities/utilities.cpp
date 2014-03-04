#include "utilities.h"


#include <stdexcept>

std::shared_ptr<AbstractLogger> GlobalUtilities::_logger = std::shared_ptr<AbstractLogger>(new DummyLogger());

void GlobalUtilities::setLogger(std::shared_ptr<AbstractLogger> logger)
{

    if(!logger)
        throw std::invalid_argument("GlobalUtilities::setLogger(logger) with logger == nullptr.");
    _logger.swap(logger);
}

std::shared_ptr<AbstractLogger> GlobalUtilities::getLogger()
{
    return _logger;
}
