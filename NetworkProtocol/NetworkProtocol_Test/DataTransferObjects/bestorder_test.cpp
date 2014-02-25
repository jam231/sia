#include "bestorder_test.h"

#include <DataTransferObjects/types.h>

#include <utilities.h>
#include <stdexcept>

using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;



void BestOrderTest::initTestCase()
{
    GlobalUtilities::setLogger(make_logger(LoggingLevel::Off));
}

void BestOrderTest::creation_valid_data()
{
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 60000; i += 10000)
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 99)
                                         << AmountType(100) << PriceType(100);

    for(int i = 1; i < 60000; i += 10000)
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 99)
                                        << AmountType(100) << PriceType(100);

}

Q_DECLARE_METATYPE(OrderType)
Q_DECLARE_METATYPE(StockIdType)
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)

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
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 0; i < 60; i += 10)
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(-i)
                                        << AmountType(i) << PriceType(i);
    for(int i = 0; i < 60; i += 10)
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 20)
                                        << AmountType(-i) << PriceType(i);
    for(int i = 0; i < 60; i += 10)
        QTest::newRow("BUY best order") << OrderType::BUY << StockIdType(i % 10)
                                        << AmountType(i) << PriceType(-i);

    for(int i = 1; i < 60; i += 10)
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(-i)
                                         << AmountType(i) << PriceType(i);
    for(int i = 1; i < 60; i += 10)
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 20)
                                         << AmountType(-i) << PriceType(i);
    for(int i = 1; i < 60; i += 10)
        QTest::newRow("SELL best order") << OrderType::SELL << StockIdType(i % 10)
                                         << AmountType(i) << PriceType(-i);

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

void BestOrderTest::operatorLSH_valid_data()
{
    /// STUB
}

void BestOrderTest::operatorLSH_valid()
{
    /// STUB
}

void BestOrderTest::operatorLSH_invalid_data()
{
    /// STUB
}

void BestOrderTest::operatorLSH_invalid()
{
    /// STUB
}

void BestOrderTest::operatorRSH_valid_data()
{
    /// STUB
}


void BestOrderTest::operatorRSH_valid()
{
    /// STUB
}

void BestOrderTest::operatorRSH_invalid_data()
{
    /// STUB
}


void BestOrderTest::operatorRSH_invalid()
{
    /// STUB
}

void BestOrderTest::lengthInBytes_data()
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

void BestOrderTest::lengthInBytes()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    BestOrder best_order(order_type, stock_id, amount, price);

    Message::MessageLengthType sum_of_sizeofs = sizeof(order_type) + sizeof(stock_id) +
                                                sizeof(amount) + sizeof(price);

    QVERIFY2(best_order.lengthInBytes() == sum_of_sizeofs, "Best order length in bytes is incorrect.");
}

