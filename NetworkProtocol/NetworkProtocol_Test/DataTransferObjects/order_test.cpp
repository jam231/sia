#include "order_test.h"

#include <DataTransferObjects/types.h>

#include <utilities.h>

#include <stdexcept>
#include <assert.h>
#include <memory>

using namespace NetworkProtocol;
using namespace DTO;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(OrderIdType)
Q_DECLARE_METATYPE(OrderType)
Q_DECLARE_METATYPE(StockIdType)
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)


void OrderTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}


void OrderTest::generate_valid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 60000; i += 10000)
    {
        QTest::newRow("SELL order")      << OrderIdType(i % 99) << OrderType::SELL
                                         << StockIdType(i % 99) << AmountType(100)
                                         << PriceType(100);
    }
    for(int i = 1; i < 60000; i += 10000)
    {
        QTest::newRow("BUY order")      << OrderIdType(i % 99) << OrderType::BUY
                                        << StockIdType(i % 99) << AmountType(100)
                                        << PriceType(100);
    }
}
void OrderTest::generate_invalid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY order") << OrderIdType(0)     << OrderType::BUY
                                   << StockIdType(i % 20) << AmountType(i)
                                   << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY order") << OrderIdType(i % 20) << OrderType::BUY
                                   << StockIdType(0)     << AmountType(i)
                                   << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY order") << OrderIdType(i % 20) << OrderType::BUY
                                   << StockIdType(i % 20) << AmountType(0)
                                   << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY order") << OrderIdType(i % 20) << OrderType::BUY
                                   << StockIdType(i % 10) << AmountType(i)
                                   << PriceType(0);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL order") << OrderIdType(0)     << OrderType::SELL
                                    << StockIdType(i % 20) << AmountType(i)
                                    << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL order") << OrderIdType(i % 20) << OrderType::SELL
                                    << StockIdType(0)     << AmountType(i)
                                    << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL order") << OrderIdType(i % 20) << OrderType::SELL
                                    << StockIdType(i % 20) << AmountType(0)
                                    << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL order") << OrderIdType(i % 20) << OrderType::SELL
                                    << StockIdType(i % 10) << AmountType(i)
                                    << PriceType(0);
    }
}


void OrderTest::creation_valid_data()
{
    generate_valid_data();
}

void OrderTest::creation_valid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);
    try
    {
        DTO::Order order(order_id, order_type, stock_id, amount, price);
        QVERIFY2(order.getOrderId()    == order_id,    "Order id is corrupted.");
        QVERIFY2(order.getOrderType()  == order_type,  "Order type is corrupted.");
        QVERIFY2(order.getStockId()    == stock_id,    "Stock id is corrupted.");
        QVERIFY2(order.getAmount()     == amount,      "Amount is corrupted.");
        QVERIFY2(order.getPrice()      == price,       "Price corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}


void OrderTest::creation_invalid_data()
{
    generate_invalid_data();
}

void OrderTest::creation_invalid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        DTO::Order order(order_id, order_type, stock_id, amount, price);
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

void OrderTest::toStream_valid_data()
{
    generate_valid_data();
}

void OrderTest::toStream_valid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        DTO::Order order(order_id, order_type, stock_id, amount, price);
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << order;
        stream.device()->reset();
        qint32 should_be_bytes = sizeof(order_id) + sizeof(order_type) +
                                 sizeof(stock_id) + sizeof(amount) + sizeof(price);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes).arg(stream.device()->size())));
        OrderIdType should_be_order_id;
        OrderType   should_be_order_type;
        StockIdType should_be_stock_id;
        AmountType  should_be_amount;
        PriceType   should_be_price;
        stream >> should_be_order_id >> should_be_order_type
               >> should_be_stock_id >> should_be_amount >> should_be_price;

        QVERIFY2(should_be_order_id == order_id,
                 qPrintable(QString("Order id doesn't match. Is %1 should be %2")
                            .arg(should_be_order_id.value)
                            .arg(order_id.value)));
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


void OrderTest::fromStream_valid_data()
{
    generate_valid_data();
}

void OrderTest::fromStream_valid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << order_id << order_type << stock_id << amount << price;
        stream.device()->reset();

        Types::Message::MessageLengthType should_be_bytes = sizeof(OrderIdType) +
                sizeof(OrderType) + sizeof(StockIdType) + sizeof(AmountType) +
                sizeof(PriceType);

        QVERIFY2(stream.device()->bytesAvailable() == should_be_bytes,
                 qPrintable(QString("Bytes available in device doesn't equal to "
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        DTO::Order from_stream = DTO::Order::fromStream(stream);

        QVERIFY2(from_stream.getOrderId() == order_id,
                 qPrintable(QString("Order id doesn't match. Is %1 should be %2")
                            .arg(from_stream.getOrderId().value)
                            .arg(order_id.value)));
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

void OrderTest::fromStream_invalid_data()
{
    generate_invalid_data();
}

void OrderTest::fromStream_invalid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << order_id << order_type << stock_id << amount << price;
        stream.device()->reset();

        Types::Message::MessageLengthType should_be_bytes = sizeof(OrderIdType) +
                sizeof(OrderType) + sizeof(StockIdType) + sizeof(AmountType) +
                sizeof(PriceType);

        QVERIFY2(stream.device()->bytesAvailable() == should_be_bytes,
                 qPrintable(QString("Bytes available in device doesn't equal to"\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        DTO::Order::fromStream(stream);
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

void OrderTest::lengthSerialized_data()
{
    generate_valid_data();
}

void OrderTest::lengthSerialized()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    DTO::Order order(order_id, order_type, stock_id, amount, price);

    Message::MessageLengthType sum_of_sizeofs = sizeof(order_id) + sizeof(order_type) +
                                                sizeof(stock_id) + sizeof(amount) +
                                                sizeof(price);

    QVERIFY2(order.lengthSerialized() == sum_of_sizeofs,
             qPrintable(QString("Order length in bytes is incorrect."\
                                "Is %1 should be %2.")
                        .arg(order.lengthSerialized()).arg(sum_of_sizeofs)));

}
