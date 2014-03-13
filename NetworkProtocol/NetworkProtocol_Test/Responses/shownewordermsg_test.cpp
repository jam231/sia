#include "shownewordermsg_test.h"

#include <Responses/shownewordermsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(OrderIdType)
Q_DECLARE_METATYPE(OrderType)
Q_DECLARE_METATYPE(StockIdType)
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)


void ShowNewOrderTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}

void ShowNewOrderTest::generate_valid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 60000; i += 10000)
    {
        QTest::newRow("SELL best order") << OrderIdType(i % 99)
                                         << OrderType::SELL << StockIdType(i % 99)
                                         << AmountType(100) << PriceType(100);
    }
    for(int i = 1; i < 60000; i += 10000)
    {
        QTest::newRow("BUY best order") << OrderIdType(i % 99)
                                        << OrderType::BUY << StockIdType(i % 99)
                                        << AmountType(100) << PriceType(100);
    }
}

void ShowNewOrderTest::generate_invalid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderIdType(0)
                                       << OrderType::BUY << StockIdType(i)
                                       << AmountType(i) << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderIdType(i)
                                       << OrderType::BUY << StockIdType(0)
                                       << AmountType(i) << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderIdType(i)
                                        << OrderType::BUY << StockIdType(i % 20)
                                        << AmountType(0) << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QTest::newRow("BUY best order") << OrderIdType(i)
                                        << OrderType::BUY << StockIdType(i % 10)
                                        << AmountType(i) << PriceType(0);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderIdType(0)
                                         << OrderType::SELL << StockIdType(i)
                                         << AmountType(i) << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderIdType(i)
                                         << OrderType::SELL << StockIdType(0)
                                         << AmountType(i) << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderIdType(i)
                                         << OrderType::SELL << StockIdType(i % 20)
                                         << AmountType(0) << PriceType(i);
    }
    for(int i = 1; i < 60; i += 10)
    {
        QTest::newRow("SELL best order") << OrderIdType(i)
                                         << OrderType::SELL << StockIdType(i % 10)
                                         << AmountType(i) << PriceType(0);
    }
}

void ShowNewOrderTest::creation_valid_data()
{
    generate_valid_data();
}
void ShowNewOrderTest::creation_valid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        Responses::ShowNewOrder show_new_order(order_id, order_type, stock_id,
                                               amount, price);

        QVERIFY2(show_new_order.getOrder().getOrderId()  == order_id,
                 "Order id is corrupted.");
        QVERIFY2(show_new_order.getOrder().getOrderType()  == order_type,
                 "Order type is corrupted.");
        QVERIFY2(show_new_order.getOrder().getStockId()    == stock_id,
                 "Stock id is corrupted.");
        QVERIFY2(show_new_order.getOrder().getAmount()     == amount,
                 "Amount is corrupted.");
        QVERIFY2(show_new_order.getOrder().getPrice()      == price,
                 "Price corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}




void ShowNewOrderTest::creation_invalid_data()
{
    generate_invalid_data();
}

void ShowNewOrderTest::creation_invalid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        Responses::ShowNewOrder show_new_order(order_id, order_type, stock_id,
                                               amount, price);

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


void ShowNewOrderTest::constant_length_data()
{
    generate_valid_data();
}

void ShowNewOrderTest::constant_length()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        Responses::ShowNewOrder show_new_order(order_id, order_type, stock_id,
                                               amount, price);


        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   show_new_order.getOrder()
                                                                 .lengthSerialized();

        QVERIFY2(show_new_order.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(show_new_order.length())));

        QVERIFY2(show_new_order.length() == 20,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(20)
                            .arg(show_new_order.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void ShowNewOrderTest::send_data()
{
    generate_valid_data();
}

void ShowNewOrderTest::send()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        Responses::ShowNewOrder show_new_order(order_id, order_type, stock_id,
                                               amount, price);
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        show_new_order.send(stream.device());

        stream.device()->reset();


        Message::MessageLengthType should_be_bytes =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   show_new_order.getOrder()
                                                                  .lengthSerialized();

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        OrderIdType                is_order_id;
        OrderType                  is_order_type;
        StockIdType                is_stock_id;
        AmountType                 is_amount;
        PriceType                  is_price;

        stream >> is_length >> is_type >> is_order_id >> is_order_type
               >> is_stock_id >> is_amount >> is_price;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_SHOW_NEW_ORDER == 0x19);

        QVERIFY2(is_type == Message::RESPONSE_SHOW_NEW_ORDER,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_SHOW_NEW_ORDER)));

        QVERIFY2(is_order_id == order_id,
                 qPrintable(QString("Order id doesn't match. Is %1 should be %2.")
                            .arg(is_order_id.value)
                            .arg(order_id.value)));
        QVERIFY2(is_order_type     == order_type,

                 qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                            .arg(is_order_type)
                            .arg(order_type)));

        QVERIFY2(is_stock_id == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(is_stock_id.value)
                            .arg(stock_id.value)));

        QVERIFY2(is_amount     == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                            .arg(is_amount.value)
                            .arg(amount.value)));

        QVERIFY2(is_price      == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                            .arg(is_price.value)
                            .arg(price.value)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}
