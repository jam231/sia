#include "orderrealizationmsg_test.h"

#include <Responses/orderrealizationmsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;


Q_DECLARE_METATYPE(OrderIdType)
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)

void OrderRealizationTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void OrderRealizationTest::generate_valid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(long long i = 2; i <= 32; i += 2)
    {
        OrderIdType order_id = OrderIdType((static_cast<long long>(1) << i) - 1);
        AmountType amount    = AmountType((static_cast<long long>(1) << i) - 1);
        PriceType price      = PriceType((static_cast<long long>(1) << i) - 1);

        QTest::newRow(qPrintable(QString("values = 2 ^ %1 - 1= %1")
                                 .arg(i)))
                << order_id << amount << price;
    }
}

void OrderRealizationTest::creation_invalid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    QTest::newRow("order id = 0")  << OrderIdType(0) << AmountType(1) << PriceType(1);
    QTest::newRow("price id = 0")  << OrderIdType(1) << AmountType(1) << PriceType(0);
    QTest::newRow("amount id = 0") << OrderIdType(1) << AmountType(0) << PriceType(1);

}

void OrderRealizationTest::creation_invalid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(AmountType,  amount);
    QFETCH(PriceType,  price);

    try
    {
        Responses::OrderRealization order_realization(order_id, amount, price);

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

void OrderRealizationTest::creation_valid_data()
{
    generate_valid_data();
}

void OrderRealizationTest::creation_valid()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(AmountType,  amount);
    QFETCH(PriceType,  price);

    try
    {
        Responses::OrderRealization order_realization(order_id, amount, price);

        QVERIFY2(order_realization.getOrderId()  == order_id,
                 "Order id is corrupted.");
        QVERIFY2(order_realization.getAmount()  == amount,
                 "Amount is corrupted.");
        QVERIFY2(order_realization.getPrice()  == price,
                 "Price is corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}

void OrderRealizationTest::constant_length_data()
{
    generate_valid_data();
}

void OrderRealizationTest::constant_length()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(AmountType,  amount);
    QFETCH(PriceType,  price);

    try
    {
        Responses::OrderRealization order_realization(order_id, amount, price);

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(OrderIdType) +
                                                   sizeof(AmountType) +
                                                   sizeof(PriceType);

        QVERIFY2(order_realization.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(order_realization.length())));
        QVERIFY2(order_realization.length() == 15,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(15)
                            .arg(order_realization.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void OrderRealizationTest::send_data()
{
    generate_valid_data();
}

void OrderRealizationTest::send()
{
    QFETCH(OrderIdType, order_id);
    QFETCH(AmountType,  amount);
    QFETCH(PriceType,  price);

    try
    {
        Responses::OrderRealization order_realization(order_id, amount, price);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        order_realization.send(stream.device());

        stream.device()->reset();

        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                     sizeof(Message::MessageType) +
                                                     sizeof(OrderIdType) +
                                                     sizeof(AmountType) +
                                                     sizeof(PriceType);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        QVERIFY2(stream.device()->size() == 15,
                 qPrintable(QString("Bytes saved in device doesn't match with"\
                                    "specification. Should be %1 is %2.")
                            .arg(15)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        OrderIdType                is_order_id;
        AmountType                 is_amount;
        PriceType                  is_price;

        stream >> is_length >> is_type >> is_order_id >> is_amount >> is_price;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_ORDER_REALIZATION == 0x17);

                QVERIFY2(is_type == Message::RESPONSE_ORDER_REALIZATION,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_ORDER_REALIZATION)));

        QVERIFY2(is_order_id == order_id,
                 qPrintable(QString("User id doesn't match. Is %1 should be %2.")
                            .arg(is_order_id.value)
                            .arg(order_id.value)));
        QVERIFY2(is_amount == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                            .arg(is_amount.value)
                            .arg(amount.value)));
        QVERIFY2(is_price == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                            .arg(is_price.value)
                            .arg(price.value)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

