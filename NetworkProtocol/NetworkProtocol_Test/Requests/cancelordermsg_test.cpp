#include "cancelordermsg_test.h"

#include <Requests/cancelordermsg.h>
#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

Q_DECLARE_METATYPE(QByteArray)

Q_DECLARE_METATYPE(OrderIdType)

void CancelOrderTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void CancelOrderTest::from_valid_data()
{
    QTest::addColumn<OrderIdType>("order_id");

    for(long long i = 2; i <= 32; i += 2)
    {
        OrderIdType order_id = OrderIdType((static_cast<long long>(1) << i) - 1);

        QTest::newRow(qPrintable(QString("order id = 2 ^ %1 - 1")
                                 .arg(i)))
                << order_id;
    }
}

void CancelOrderTest::from_valid()
{
    QFETCH(OrderIdType, order_id);
    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType) +
                sizeof(order_id);

        assert (length == 7);
        assert (Message::REQUEST_CANCEL_ORDER == 0x23);

        stream << order_id;

        stream.device()->reset();

        Requests::CancelOrder cancel_order(stream);

        QVERIFY2(cancel_order.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(cancel_order.length())
                            .arg(length)));

        QVERIFY2(cancel_order.getOrderId() == order_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(cancel_order.getOrderId().value)
                            .arg(order_id.value)));

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

void CancelOrderTest::from_invalid_body_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);


    stream << OrderIdType(0);
    stream.device()->reset();

    Message::MessageLengthType should_be_length = sizeof(OrderIdType);

    assert(stream.device()->bytesAvailable() == should_be_length);

    QTest::newRow("Invalid data") << buffer;
}

void CancelOrderTest::from_invalid_body()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::CancelOrder cancel_order(stream);
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

void CancelOrderTest::from_malformed_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    typedef quint32 underlying_type;

    assert(sizeof(OrderIdType) == sizeof(underlying_type));

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);

    assert(stream.device()->bytesAvailable() == 0 * sizeof(underlying_type));
    QTest::newRow("Invalid data") << buffer;
}

void CancelOrderTest::from_malformed()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::CancelOrder cancel_order(stream);
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
