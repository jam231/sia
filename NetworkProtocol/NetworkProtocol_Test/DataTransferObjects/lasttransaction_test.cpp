#include "lasttransaction_test.h"

#include <DataTransferObjects/types.h>

#include <utilities.h>
#include <stdexcept>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;

/// TODO:
///     Impelement tests

void LastTransactionTest::initTestCase()
{
    GlobalUtilities::setLogger(make_logger(LoggingLevel::Off));
}


void LastTransactionTest::creation_valid_data()
{
}

void LastTransactionTest::creation_valid()
{
    QSKIP("Not implemented yet.");
}


void LastTransactionTest::creation_invalid_data()
{
}

void LastTransactionTest::creation_invalid()
{
    QSKIP("Not implemented yet.");

}

void LastTransactionTest::toStream_valid_data()
{
}

void LastTransactionTest::toStream_valid()
{
    QSKIP("Not implemented yet.");

}


void LastTransactionTest::fromStream_valid_data()
{
}


void LastTransactionTest::fromStream_valid()
{
    QSKIP("Not implemented yet.");
}

void LastTransactionTest::fromStream_invalid_data()
{
}


void LastTransactionTest::fromStream_invalid()
{
    QSKIP("Not implemented yet.");
}

void LastTransactionTest::lengthSerialized_data()
{
}

void LastTransactionTest::lengthSerialized()
{
    QSKIP("Not implemented yet.");
}

