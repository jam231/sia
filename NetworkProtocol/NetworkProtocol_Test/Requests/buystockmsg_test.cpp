#include "buystockmsg_test.h"

#include <Requests/buystockmsg.h>
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
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)


void BuyStockTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void BuyStockTest::from_valid_data()
{
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(long long i = 2; i <= 32; i += 2)
    {
        StockIdType stock_id = StockIdType((static_cast<long long>(1) << i) - 1);
        AmountType amount    = AmountType((static_cast<long long>(1) << i) - 1);
        PriceType price      = PriceType((static_cast<long long>(1) << i) - 1);

        QTest::newRow(qPrintable(QString("values = 2 ^ %1 - 1")
                                 .arg(i)))
                << stock_id << amount << price;
    }
}

void BuyStockTest::from_valid()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType,  amount);
    QFETCH(PriceType,  price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType) +
                sizeof(stock_id) + sizeof(amount) + sizeof(price);

        assert (length == 15);
        assert (Message::REQUEST_BUY_STOCK_ORDER == 0x15);

        stream << stock_id << amount << price;

        stream.device()->reset();

        Requests::BuyStock buy_stock(stream);

        QVERIFY2(buy_stock.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(buy_stock.length())
                            .arg(length)));

        QVERIFY2(buy_stock.getStockId() == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(buy_stock.getStockId().value)
                            .arg(stock_id.value)));
        QVERIFY2(buy_stock.getAmount() == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                            .arg(buy_stock.getAmount().value)
                            .arg(amount.value)));
        QVERIFY2(buy_stock.getPrice() == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                            .arg(buy_stock.getPrice().value)
                            .arg(price.value)));
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

void BuyStockTest::from_invalid_body_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);


    stream << StockIdType(0) << AmountType(0) << PriceType(0);
    stream.device()->reset();

    Message::MessageLengthType should_be_length = sizeof(StockIdType) +
            sizeof(AmountType) + sizeof(PriceType);

    assert(stream.device()->bytesAvailable() == should_be_length);

    QTest::newRow("Invalid data") << buffer;

    for(int i = 2; i < 5; i++)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);


        stream << StockIdType(i % 2) << AmountType(i % 3) << PriceType(i % 4);
        stream.device()->reset();
        assert(stream.device()->bytesAvailable() == should_be_length);
        QTest::newRow("Invalid data") << buffer;
    }
}

void BuyStockTest::from_invalid_body()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::BuyStock buy_stock(stream);
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

void BuyStockTest::from_malformed_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    typedef quint32  underlying_type;

    assert(sizeof(StockIdType) == sizeof(underlying_type));
    assert(sizeof(AmountType) == sizeof(underlying_type));
    assert(sizeof(PriceType) == sizeof(underlying_type));

    for(int field_count = 0; field_count < 3; field_count++)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        for(int i = 0; i < field_count; i++)
        {
            stream << underlying_type(1);
        }
        stream.device()->reset();
        assert(stream.device()->bytesAvailable() == field_count * sizeof(underlying_type));
        QTest::newRow("Invalid data") << buffer;
    }
}

void BuyStockTest::from_malformed()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::BuyStock buy_stock(stream);
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
