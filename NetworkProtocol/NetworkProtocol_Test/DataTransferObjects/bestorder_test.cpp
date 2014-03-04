#include "bestorder_test.h"

#include <DataTransferObjects/types.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(OrderType)
Q_DECLARE_METATYPE(StockIdType)
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)


void BestOrderTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}

void BestOrderTest::generate_valid_data()
{
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 60000; i += 10000)
    {
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 99)
                                         << AmountType(100) << PriceType(100);
    }
    for(int i = 1; i < 60000; i += 10000)
    {
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 99)
                                        << AmountType(100) << PriceType(100);
    }
}
void BestOrderTest::generate_invalid_data()
{
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(-i)
                                       << AmountType(i) << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 20)
                                        << AmountType(-i) << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 10)
                                        << AmountType(i) << PriceType(-i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(-i)
                                         << AmountType(i) << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 20)
                                         << AmountType(-i) << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 10)
                                         << AmountType(i) << PriceType(-i);
    }
}

void BestOrderTest::creation_valid_data()
{
    generate_valid_data();
}

void BestOrderTest::creation_valid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        BestOrder best_order(order_type, stock_id, amount, price);
        QVERIFY2(best_order.getOrderType()  == order_type,  "Order type is corrupted.");
        QVERIFY2(best_order.getStockId()    == stock_id,    "Stock id is corrupted.");
        QVERIFY2(best_order.getAmount()     == amount,      "Amount is corrupted.");
        QVERIFY2(best_order.getPrice()      == price,       "Price corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}


void BestOrderTest::creation_invalid_data()
{
    generate_invalid_data();
}

void BestOrderTest::creation_invalid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        BestOrder best_order(order_type, stock_id, amount, price);
        QFAIL("std::invalid_argument should have been thrown.");
    }
    catch(std::invalid_argument&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Unkown exception has been thrown");
    }
}

void BestOrderTest::toStream_valid_data()
{
    generate_valid_data();
}

void BestOrderTest::toStream_valid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        BestOrder best_order(order_type, stock_id, amount, price);
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << best_order;
        stream.device()->reset();
        qint32 should_be_bytes = sizeof(order_type) + sizeof(stock_id) +
                                 sizeof(amount) + sizeof(price);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes).arg(stream.device()->size())));

        OrderType   should_be_order_type;
        StockIdType should_be_stock_id;
        AmountType  should_be_amount;
        PriceType   should_be_price;
        stream >> should_be_order_type >> should_be_stock_id
               >> should_be_amount >> should_be_price;

        QVERIFY2(should_be_order_type == order_type,
                 qPrintable(QString("Order type doesn't match. Is %1 should be %2")
                            .arg(should_be_order_type)
                            .arg(order_type)));
        QVERIFY2(should_be_stock_id == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2")
                            .arg(should_be_stock_id.value)
                            .arg(stock_id.value)));
        QVERIFY2(should_be_amount == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2")
                            .arg(should_be_amount.value)
                            .arg(amount.value)));
        QVERIFY2(should_be_price == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2")
                            .arg(should_be_price.value)
                            .arg(price.value)));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}


void BestOrderTest::fromStream_valid_data()
{
    generate_valid_data();
}


void BestOrderTest::fromStream_valid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << order_type << stock_id << amount << price;
        stream.device()->reset();

        Types::Message::MessageLengthType should_be_bytes = sizeof(OrderType) +
                sizeof(StockIdType) + sizeof(AmountType) + sizeof(PriceType);

        QVERIFY2(stream.device()->bytesAvailable() == should_be_bytes,
                 qPrintable(QString("Bytes available in device doesn't equal to "
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        BestOrder from_stream = BestOrder::fromStream(stream);

        QVERIFY2(from_stream.getOrderType() == order_type,
                 qPrintable(QString("Order type doesn't match. Is %1 should be %2")
                            .arg(from_stream.getOrderType())
                            .arg(order_type)));
        QVERIFY2(from_stream.getStockId() == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2")
                            .arg(from_stream.getStockId().value)
                            .arg(stock_id.value)));
        QVERIFY2(from_stream.getAmount() == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2")
                            .arg(from_stream.getAmount().value)
                            .arg(amount.value)));
        QVERIFY2(from_stream.getPrice() == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2")
                            .arg(from_stream.getPrice().value)
                            .arg(price.value)));

    }
    catch(std::exception& e)
    {
        QFAIL(qPrintable(QString("Exception has been thrown : %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unkown exception has been thrown.");
    }
}

void BestOrderTest::fromStream_invalid_data()
{
    generate_invalid_data();
}


void BestOrderTest::fromStream_invalid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << order_type << stock_id << amount << price;
        stream.device()->reset();

        Types::Message::MessageLengthType should_be_bytes =
                sizeof(OrderType) + sizeof(StockIdType) + sizeof(AmountType) +
                sizeof(PriceType);

        QVERIFY2(stream.device()->bytesAvailable() == should_be_bytes,
                 qPrintable(QString("Bytes available in device doesn't equal to"\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        BestOrder::fromStream(stream);
        QFAIL("std::invalid_argument should have been thrown.");

    }
    catch(std::invalid_argument&)
    {
        // This should have been thrown.
        return;
    }
    catch(...)
    {
        QFAIL("Unkown exception has been thrown.");
    }
}

void BestOrderTest::lengthSerialized_data()
{
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 15000; i += 1000)
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 99)
                                         << AmountType(i) << PriceType(i);

    for(int i = 1; i < 15000; i += 1000)
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 99)
                                        << AmountType(i) << PriceType(i);

}

void BestOrderTest::lengthSerialized()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    BestOrder best_order(order_type, stock_id, amount, price);

    Message::MessageLengthType sum_of_sizeofs = sizeof(order_type) + sizeof(stock_id) +
                                                sizeof(amount) + sizeof(price);

    QVERIFY2(best_order.lengthSerialized() == sum_of_sizeofs,
             qPrintable(QString("Best order length in bytes is incorrect."\
                                "Is %1 should be %2.")
                        .arg(best_order.lengthSerialized()).arg(sum_of_sizeofs)));

}

