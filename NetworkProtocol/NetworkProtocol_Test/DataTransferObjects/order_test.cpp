#include "order_test.h"

#include <DataTransferObjects/types.h>

#include <utilities.h>
#include <stdexcept>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;

/// TODO:
///     Impelement tests

void OrderTest::initTestCase()
{
    GlobalUtilities::setLogger(make_logger(LoggingLevel::Off));
}


void OrderTest::creation_valid_data()
{
}

void OrderTest::creation_valid()
{
    QSKIP("Not implemented yet.");
}


void OrderTest::creation_invalid_data()
{
}

void OrderTest::creation_invalid()
{
    QSKIP("Not implemented yet.");

}

void OrderTest::toStream_valid_data()
{
}

void OrderTest::toStream_valid()
{
    QSKIP("Not implemented yet.");

}


void OrderTest::fromStream_valid_data()
{
}


void OrderTest::fromStream_valid()
{
    QSKIP("Not implemented yet.");
}

void OrderTest::fromStream_invalid_data()
{
}


void OrderTest::fromStream_invalid()
{
    QSKIP("Not implemented yet.");
}

void OrderTest::lengthSerialized_data()
{
}

void OrderTest::lengthSerialized()
{
    QSKIP("Not implemented yet.");
}
