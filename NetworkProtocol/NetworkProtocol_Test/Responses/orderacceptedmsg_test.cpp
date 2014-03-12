#include "orderacceptedmsg_test.h"

#include <Responses/orderacceptedmsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;


Q_DECLARE_METATYPE(OrderIdType)


void OrderAcceptedTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void OrderAcceptedTest::generate_valid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    for(long long i = 2; i <= 32; i += 2)
    {
        OrderIdType order_id = OrderIdType((static_cast<long long>(1) << i) - 1);
        QTest::newRow(qPrintable(QString("order id = 2 ^ %1 - 1= %2")
                                 .arg(i)
                                 .arg(order_id.value)))
                << order_id;
    }
}

void OrderAcceptedTest::creation_invalid_data()
{
    QTest::addColumn<OrderIdType>("order_id");
    QTest::newRow("order id = 0") << OrderIdType(0);
}

void OrderAcceptedTest::creation_invalid()
{
    QFETCH(OrderIdType, order_id);

    try
    {
        Responses::OrderAccepted order_accepted(order_id);

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

void OrderAcceptedTest::creation_valid_data()
{
    generate_valid_data();
}

void OrderAcceptedTest::creation_valid()
{
    QFETCH(OrderIdType, order_id);

    try
    {
        Responses::OrderAccepted order_accepted(order_id);

        QVERIFY2(order_accepted.getOrderId()  == order_id,
                 "Order id is corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}

void OrderAcceptedTest::constant_length_data()
{
    generate_valid_data();
}

void OrderAcceptedTest::constant_length()
{
    QFETCH(OrderIdType, order_id);

    try
    {
        Responses::OrderAccepted order_accepted(order_id);

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(OrderIdType);

        QVERIFY2(order_accepted.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(order_accepted.length())));
        QVERIFY2(order_accepted.length() == 7,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(7)
                            .arg(order_accepted.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void OrderAcceptedTest::send_data()
{
    generate_valid_data();
}

void OrderAcceptedTest::send()
{
    QFETCH(OrderIdType, order_id);

    try
    {
        Responses::OrderAccepted order_accepted(order_id);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        order_accepted.send(stream.device());

        stream.device()->reset();

        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                     sizeof(Message::MessageType) +
                                                     sizeof(OrderIdType);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        QVERIFY2(stream.device()->size() == 7,
                 qPrintable(QString("Bytes saved in device doesn't match with"\
                                    "specification. Should be %1 is %2.")
                            .arg(7)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        OrderIdType                is_order_id;

        stream >> is_length >> is_type >> is_order_id;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_ORDER_ACCEPTED == 0x24);

                QVERIFY2(is_type == Message::RESPONSE_ORDER_ACCEPTED,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_ORDER_ACCEPTED)));

        QVERIFY2(is_order_id == order_id,
                 qPrintable(QString("User id doesn't match. Is %1 should be %2.")
                            .arg(is_order_id.value)
                            .arg(order_id.value)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}
