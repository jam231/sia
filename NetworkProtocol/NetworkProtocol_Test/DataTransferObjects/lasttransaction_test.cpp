#include "lasttransaction_test.h"

#include <DataTransferObjects/types.h>

#include <utilities.h>
#include <stdexcept>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;

/// TODO:
///     Implement tests

Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)

void LastTransactionTest::initTestCase()
{
    GlobalUtilities::setLogger(make_logger(LoggingLevel::Off));
}
void LastTransactionTest::generate_valid_data()
{
    QTest::addColumn<QString>("date_time");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 60000; i += 10000)
    {
        QString date_time = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("valid data") << date_time << AmountType(i) << PriceType(i + i % 60);
    }
}
void LastTransactionTest::generate_invalid_data()
{
    QTest::addColumn<QString>("date_time");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 0; i < 60; i += 10)
    {
        QString date_time = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("invalid data") << date_time << AmountType(-i) << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QString date_time = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("invalid data") << date_time << AmountType(i) << PriceType(-i);
    }
}

void LastTransactionTest::creation_valid_data()
{
    generate_valid_data();
}

void LastTransactionTest::creation_valid()
{
    QSKIP("Not implemented yet.");
}


void LastTransactionTest::creation_invalid_data()
{
    generate_invalid_data();
}

void LastTransactionTest::creation_invalid()
{
    QSKIP("Not implemented yet.");

}

void LastTransactionTest::toStream_valid_data()
{
    generate_valid_data();
}

void LastTransactionTest::toStream_valid()
{
    QSKIP("Not implemented yet.");

}


void LastTransactionTest::fromStream_valid_data()
{
    generate_invalid_data();
}


void LastTransactionTest::fromStream_valid()
{
    QSKIP("Not implemented yet.");
}

void LastTransactionTest::fromStream_invalid_data()
{
    generate_invalid_data();
}


void LastTransactionTest::fromStream_invalid()
{
    QSKIP("Not implemented yet.");
}

void LastTransactionTest::lengthSerialized_data()
{
    generate_valid_data();
}

void LastTransactionTest::lengthSerialized()
{
    QSKIP("Not implemented yet.");
}

