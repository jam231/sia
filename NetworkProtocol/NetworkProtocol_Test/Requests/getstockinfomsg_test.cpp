#include "getstockinfomsg_test.h"

#include <Requests/getstockinfomsg.h>
#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(StockIdType)

void GetStockInfoTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void GetStockInfoTest::from_valid_data()
{
    QTest::addColumn<StockIdType>("stock_id");

    for(long long i = 2; i <= 32; i += 2)
    {
        StockIdType stock_id = StockIdType((static_cast<long long>(1) << i) - 1);

        QTest::newRow(qPrintable(QString("stock id = 2 ^ %1 - 1")
                                 .arg(i)))
                << stock_id;
    }
}

void GetStockInfoTest::from_valid()
{
    QFETCH(StockIdType, stock_id);
    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType) +
                sizeof(stock_id);

        assert (length == 7);
        assert (Message::REQUEST_GET_STOCK_INFO == 0x21);

        stream << stock_id;

        stream.device()->reset();

        Requests::GetStockInfo get_stock_info(stream);

        QVERIFY2(get_stock_info.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(get_stock_info.length())
                            .arg(length)));

        QVERIFY2(get_stock_info.getStockId() == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(get_stock_info.getStockId().value)
                            .arg(stock_id.value)));

    }
    catch(Requests::InvalidRequest& e)
    {
        QFAIL(qPrintable(QString("Caught %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unkown exception has been thrown.");
    }
}

void GetStockInfoTest::from_invalid_body_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);


    stream << StockIdType(0);
    stream.device()->reset();

    Message::MessageLengthType should_be_length = sizeof(StockIdType);

    assert(stream.device()->bytesAvailable() == should_be_length);

    QTest::newRow("Invalid data") << buffer;
}

void GetStockInfoTest::from_invalid_body()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::GetStockInfo get_stock_info(stream);
    }
    catch(Requests::InvalidRequestBody&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Wrong exception has been thrown.");
    }
}

void GetStockInfoTest::from_malformed_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    typedef quint32 underlying_type;

    assert(sizeof(StockIdType) == sizeof(underlying_type));

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);

    assert(stream.device()->bytesAvailable() == 0 * sizeof(underlying_type));
    QTest::newRow("Invalid data") << buffer;
}

void GetStockInfoTest::from_malformed()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::GetStockInfo get_stock_info(stream);
    }
    catch(Requests::MalformedRequest&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Wrong exception has been thrown.");
    }
}
