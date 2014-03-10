#include "listofordersmsg_test.h"

#include <Responses/listofordersmsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol;
using namespace Types;
using namespace Order;


struct MyLittle4
{
    OrderIdType order_id;
    OrderType   order_type;
    StockIdType stock_id;
    AmountType  amount;
    PriceType   price;
};

Q_DECLARE_METATYPE(MyLittle4)

void ListOfOrdersTest::initTestCase()
{
    GlobalUtilities::setLogger(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off)));
}

void ListOfOrdersTest::generate_valid_data()
{
    QTest::addColumn<QList<MyLittle4> >("list_of_orders");

    for(int i = 1; i < 60000; i += 10000)
    {
        QList<MyLittle4> list_of_orders;
        for(int j = 0; j < 10; j++)
        {
            list_of_orders << MyLittle4 {OrderIdType(i), Types::Order::toOrderType((i % 2) + 1),
                                         StockIdType(i), AmountType(i), PriceType(i + i % 60)};
        }
        QTest::newRow("valid data") << list_of_orders;
    }
}

void ListOfOrdersTest::length_data()
{
    generate_valid_data();
}

void ListOfOrdersTest::length()
{
    QFETCH(QList<MyLittle4>, list_of_orders);


    try
    {
        Responses::ListOfOrders list_of_orders_msg;
        for(QList<MyLittle4>::iterator it = list_of_orders.begin();
            it != list_of_orders.end(); it++)
        {
            list_of_orders_msg.addOrder(it->order_id, it->order_type, it->stock_id,
                                        it->amount, it->price);
        }

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Message::MessageLengthType);
        should_be_message_length += list_of_orders.size() * (sizeof(OrderIdType) +
                                                             sizeof(OrderType) +
                                                             sizeof(StockIdType) +
                                                             sizeof(AmountType) +
                                                             sizeof(PriceType));

        QVERIFY2(list_of_orders_msg.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(list_of_orders_msg.length())));

        Message::MessageLengthType should_be_length_spec =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Message::MessageLengthType) +
                                                   17 * list_of_orders.size();
        QVERIFY2(list_of_orders_msg.length() == should_be_length_spec,
                 qPrintable(QString("Message doesn't match specification. "\
                                    "Should be %1 is %2.")
                            .arg(should_be_length_spec)
                            .arg(list_of_orders_msg.length())));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void ListOfOrdersTest::send_data()
{
    generate_valid_data();
}

void ListOfOrdersTest::send()
{
    QFETCH(QList<MyLittle4>, list_of_orders);

    try
    {
        Responses::ListOfOrders list_of_orders_msg;
        QList<std::shared_ptr<DTO::Order> > should_be_list_of_orders;

        for(QList<MyLittle4>::iterator it = list_of_orders.begin();
            it != list_of_orders.end(); it++)
        {
            std::shared_ptr<DTO::Order> order = std::shared_ptr<DTO::Order>(
                    new DTO::Order(it->order_id, it->order_type, it->stock_id,
                                   it->amount, it->price));
            list_of_orders_msg.addOrder(order);
            should_be_list_of_orders << order;
        }

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        list_of_orders_msg.send(stream.device());

        stream.device()->reset();


        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Message::MessageLengthType);
        should_be_bytes += list_of_orders.size() * (sizeof(OrderIdType) +
                                                    sizeof(OrderType) +
                                                    sizeof(StockIdType) +
                                                    sizeof(AmountType) +
                                                    sizeof(PriceType));

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        Message::MessageLengthType          is_length;
        Message::MessageType                is_type;
        Message::MessageLengthType          is_orders_count;
        QList<std::shared_ptr<DTO::Order> > is_list_of_orders;

        stream >> is_length >> is_type >> is_orders_count;
        for(int i = 0; i < is_orders_count; i++)
        {
            QDataStream tmp_stream(stream.device()->read(17));
            assert(tmp_stream.byteOrder() == QDataStream::BigEndian);
            tmp_stream.device()->reset();
            DTO::Order tmp = DTO::Order::fromStream(tmp_stream);
            is_list_of_orders << std::shared_ptr<DTO::Order>(
                                     new DTO::Order(tmp.getOrderId(), tmp.getOrderType(),
                                                    tmp.getStockId(), tmp.getAmount(),
                                                    tmp.getPrice()));
        }

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_LIST_OF_ORDERS == 0x20);

        QVERIFY2(is_type == Message::RESPONSE_LIST_OF_ORDERS,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_LIST_OF_ORDERS)));

        QVERIFY2(is_orders_count == list_of_orders.size(),
                 qPrintable(QString("Orders count match. Is %1 should be %2.")
                                    .arg(is_orders_count)
                                    .arg(list_of_orders.size())));

        for(int i = 0; i < is_orders_count; i++)
        {
            QVERIFY2(should_be_list_of_orders[i]->getOrderId() == is_list_of_orders[i]->getOrderId(),
                     qPrintable(QString("Order id doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_orders[i]->getOrderId().value)
                            .arg(should_be_list_of_orders[i]->getOrderId().value)
                            .arg(i)));
            QVERIFY2(should_be_list_of_orders[i]->getOrderType() == is_list_of_orders[i]->getOrderType(),
                     qPrintable(QString("Order Type doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_orders[i]->getOrderType())
                            .arg(should_be_list_of_orders[i]->getOrderType())
                            .arg(i)));
            QVERIFY2(should_be_list_of_orders[i]->getStockId() == is_list_of_orders[i]->getStockId(),
                     qPrintable(QString("Stock id doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_orders[i]->getStockId().value)
                            .arg(should_be_list_of_orders[i]->getStockId().value)
                            .arg(i)));
            QVERIFY2(should_be_list_of_orders[i]->getAmount() == is_list_of_orders[i]->getAmount(),
                     qPrintable(QString("Stock id doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_orders[i]->getAmount().value)
                            .arg(should_be_list_of_orders[i]->getAmount().value)
                            .arg(i)));
            QVERIFY2(should_be_list_of_orders[i]->getPrice() == is_list_of_orders[i]->getPrice(),
                     qPrintable(QString("Stock id doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_orders[i]->getPrice().value)
                            .arg(should_be_list_of_orders[i]->getPrice().value)
                            .arg(i)));
        }
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}


