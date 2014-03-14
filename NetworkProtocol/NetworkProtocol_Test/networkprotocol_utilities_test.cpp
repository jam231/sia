#include "networkprotocol_utilities_test.h"

#include <networkprotocol_utilities.h>

void NetworkProtocolUtilitiesTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}
