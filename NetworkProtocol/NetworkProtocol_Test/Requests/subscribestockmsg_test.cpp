#include "subscribestockmsg_test.h"

#include <Requests/subscribestockmsg.h>
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

void SubscribeStockTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void SubscribeStockTest::from_valid_data()
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

void SubscribeStockTest::from_valid()
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
        assert (Message::REQUEST_SUBSCRIBE_STOCK == 0x1B);

        stream << stock_id;

        stream.device()->reset();

        Requests::SubscribeStock subscribe_stock(stream);

        QVERIFY2(subscribe_stock.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(subscribe_stock.length())
                            .arg(length)));

        QVERIFY2(subscribe_stock.getStockId() == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(subscribe_stock.getStockId().value)
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

void SubscribeStockTest::from_invalid_body_data()
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

void SubscribeStockTest::from_invalid_body()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::SubscribeStock subscribe_stock(stream);
    }
    catch(Requests::InvalidRequestBodyError&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Wrong exception has been thrown.");
    }
}

void SubscribeStockTest::from_malformed_data()
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

void SubscribeStockTest::from_malformed()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::SubscribeStock subscribe_stock(stream);
    }
    catch(Requests::MalformedRequestError&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Wrong exception has been thrown.");
    }
}
