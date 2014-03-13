#include "shownobestordermsg_test.h"

#include <Responses/shownobestordermsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(OrderType)
Q_DECLARE_METATYPE(StockIdType)

void ShowNoBestOrderTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void ShowNoBestOrderTest::generate_valid_data()
{
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");

    for(long long i = 2; i <= 32; i += 2)
    {
        StockIdType stock_id = StockIdType((static_cast<long long>(1) << i) - 1);

        QTest::newRow(qPrintable(QString("stockId = 2 ^ %1 - 1= %1 and order type = %2")
                                 .arg(i).arg(toOrderType(i % 2))))
                << toOrderType(i % 2) << stock_id;
    }
}

void ShowNoBestOrderTest::creation_invalid_data()
{
    QTest::addColumn<OrderType>("order_type");
    QTest::addColumn<StockIdType>("stock_id");

    QTest::newRow("stock id = 0 and BUY ")  << Order::BUY << StockIdType(0);
    QTest::newRow("stock id = 0 and SELL")   << Order::SELL << StockIdType(0);
}

void ShowNoBestOrderTest::creation_invalid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType,  stock_id);
    try
    {
        Responses::ShowNoBestOrder show_no_best_order(order_type, stock_id);

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

void ShowNoBestOrderTest::creation_valid_data()
{
    generate_valid_data();
}

void ShowNoBestOrderTest::creation_valid()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType,  stock_id);

    try
    {
        Responses::ShowNoBestOrder show_no_best_order(order_type, stock_id);

        QVERIFY2(show_no_best_order.getOrderType()  == order_type,
                 "Order id is corrupted.");
        QVERIFY2(show_no_best_order.getStockId()  == stock_id,
                 "Amount is corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}

void ShowNoBestOrderTest::constant_length_data()
{
    generate_valid_data();
}

void ShowNoBestOrderTest::constant_length()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType,  stock_id);

    try
    {
        Responses::ShowNoBestOrder show_no_best_order(order_type, stock_id);

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(OrderType) +
                                                   sizeof(StockIdType);

        QVERIFY2(show_no_best_order.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(show_no_best_order.length())));
        QVERIFY2(show_no_best_order.length() == 8,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(8)
                            .arg(show_no_best_order.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void ShowNoBestOrderTest::send_data()
{
    generate_valid_data();
}

void ShowNoBestOrderTest::send()
{
    QFETCH(OrderType, order_type);
    QFETCH(StockIdType,  stock_id);

    try
    {
        Responses::ShowNoBestOrder show_no_best_order(order_type, stock_id);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        show_no_best_order.send(stream.device());

        stream.device()->reset();

        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                     sizeof(Message::MessageType) +
                                                     sizeof(OrderType) +
                                                     sizeof(StockIdType);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        QVERIFY2(stream.device()->size() == 8,
                 qPrintable(QString("Bytes saved in device doesn't match with"\
                                    "specification. Should be %1 is %2.")
                            .arg(8)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        OrderType                  is_order_type;
        StockIdType                is_stock_id;

        stream >> is_length >> is_type >> is_order_type >> is_stock_id;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_SHOW_NO_BEST_ORDER == 0x25);

        QVERIFY2(is_type == Message::RESPONSE_SHOW_NO_BEST_ORDER,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_SHOW_NO_BEST_ORDER)));

        QVERIFY2(is_order_type == order_type,
                 qPrintable(QString("Order type doesn't match. Is %1 should be %2.")
                            .arg(is_order_type)
                            .arg(order_type)));
        QVERIFY2(is_stock_id == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(is_stock_id.value)
                            .arg(stock_id.value)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

