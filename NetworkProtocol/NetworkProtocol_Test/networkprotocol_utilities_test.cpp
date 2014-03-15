#include "networkprotocol_utilities_test.h"

#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace Requests;
using namespace DTO;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(Message::MessageLengthType)
Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(Message::MessageType)
Q_DECLARE_METATYPE(Message::MessageTypeType)

void NetworkProtocolUtilitiesTest::initTestCase()
{
    GlobalUtilities::setLogger(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Trace,
                                                    std::shared_ptr<AbstractWriter>(new QDebugWriter()))));
}



void NetworkProtocolUtilitiesTest::getLength_valid_data()
{
    QTest::addColumn<Message::MessageLengthType>("length");

    for(long long i = 1; i <= 16; i += 2)
    {
        Message::MessageLengthType length = ((static_cast<long long>(1) << i) - 1);
        QTest::newRow(qPrintable(QString("length = 2 ^ %1 - 1= %2")
                                 .arg(i)
                                 .arg(length)))
                << length;
    }
}

void NetworkProtocolUtilitiesTest::getLength_invalid_data()
{
    QTest::addColumn<QByteArray>("binary_data");

    QByteArray buffer0;
    QTest::newRow("No data") << buffer0;

    QByteArray buffer1;
    QDataStream stream(&buffer1, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);

    assert(sizeof(quint8) == 1);
    stream << static_cast<quint8>(0);

    QTest::newRow("1 byte") << buffer1;
}

void NetworkProtocolUtilitiesTest::getLength_valid()
{
    QFETCH(Message::MessageLengthType, length);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << length;

    stream.device()->reset();


    Message::MessageLengthType is_length = Requests::getLength(stream.device());

    QVERIFY2(is_length == length,
             qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                        .arg(is_length)
                        .arg(length)));
}

void NetworkProtocolUtilitiesTest::getLength_invalid()
{
    QFETCH(QByteArray, binary_data);

    QDataStream stream(&binary_data, QIODevice::ReadWrite);

    Message::MessageLengthType is_length = Requests::getLength(stream.device());

    QVERIFY2(is_length == 0,
             qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                        .arg(is_length)
                        .arg(0)));
}

void NetworkProtocolUtilitiesTest::readLength_valid_data()
{
    getLength_valid_data();
}

void NetworkProtocolUtilitiesTest::readLength_invalid_data()
{
    getLength_invalid_data();
}

void NetworkProtocolUtilitiesTest::readLength_valid()
{
    QFETCH(Message::MessageLengthType, length);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << length;

    stream.device()->reset();
    try
    {
        assert(stream.device()->bytesAvailable() == 2);
        Message::MessageLengthType is_length = Requests::readLength(stream);

        QVERIFY2(is_length == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(length)));
        QVERIFY2(stream.device()->bytesAvailable() == 0,
                 qPrintable(QString("Incorrect number of bytes left in stream. "\
                                    "Is %1.")
                            .arg(stream.device()->bytesAvailable())));
    }
    catch(IncompleteRequest& e)
    {
        QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readLength_invalid()
{
    QFETCH(QByteArray, binary_data);

    QDataStream stream(&binary_data, QIODevice::ReadWrite);

    try
    {
        Message::MessageLengthType is_length = Requests::readLength(stream);

        QFAIL("IncompleteRequest exception should have been thrown.");
    }
    catch(IncompleteRequest& e)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readType_valid_data()
{
    QTest::addColumn<Message::MessageType>("message_type");

    QTest::newRow("REQUEST_REGISTER_USER") << Message::REQUEST_REGISTER_USER;
    QTest::newRow("REQUEST_LOGIN_USER") << Message::REQUEST_LOGIN_USER;
    QTest::newRow("REQUEST_SELL_STOCK_ORDER") << Message::REQUEST_SELL_STOCK_ORDER;
    QTest::newRow("REQUEST_BUY_STOCK_ORDER") << Message::REQUEST_BUY_STOCK_ORDER;
    QTest::newRow("REQUEST_SUBSCRIBE_STOCK") << Message::REQUEST_SUBSCRIBE_STOCK;
    QTest::newRow("REQUEST_UNSUBSCRIBE_STOCK") << Message::REQUEST_UNSUBSCRIBE_STOCK;
    QTest::newRow("REQUEST_GET_MY_STOCKS") << Message::REQUEST_GET_MY_STOCKS;
    QTest::newRow("REQUEST_GET_MY_ORDERS") << Message::REQUEST_GET_MY_ORDERS;
    QTest::newRow("REQUEST_GET_STOCK_INFO") << Message::REQUEST_GET_STOCK_INFO;
    QTest::newRow("REQUEST_CANCEL_ORDER") << Message::REQUEST_CANCEL_ORDER;

}


void NetworkProtocolUtilitiesTest::readType_invalid_data()
{
    QTest::addColumn<Message::MessageTypeType>("message_type");

    for(Message::MessageTypeType type = Message::MESSAGE_UNDEFINED; type < 250;
        type+=10)
    {
        QTest::newRow(qPrintable(QString::number(type))) << type;
    }
}

void NetworkProtocolUtilitiesTest::readType_valid()
{
    QFETCH(Message::MessageType, message_type);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << message_type;

    stream.device()->reset();
    try
    {
        assert(stream.device()->bytesAvailable() == sizeof(Message::MessageType));
        Message::MessageLengthType is_type = Requests::readType(stream);

        QVERIFY2(is_type  == message_type,
                 qPrintable(QString("Message type is incorrect. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(message_type)));
        QVERIFY2(stream.device()->bytesAvailable() == 0,
                 qPrintable(QString("Incorrect number of bytes left in stream. "\
                                    "Is %1.")
                            .arg(stream.device()->bytesAvailable())));
    }
    catch(InvalidRequestType& e)
    {
        QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readType_invalid_empty()
{
    QByteArray buffer0;
    QDataStream stream(&buffer0, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);
    assert(stream.device()->bytesAvailable() == 0);
    try
    {
        Requests::readType(stream);

        QFAIL("InvalidRequestType exception should have been thrown.");
    }
    catch(InvalidRequestType& e)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readType_invalid()
{
    QFETCH(Message::MessageTypeType, message_type);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << message_type;

    stream.device()->reset();
    try
    {
        assert(stream.device()->bytesAvailable() == sizeof(message_type));
        Requests::readType(stream);
        QFAIL("InvalidRequestType exception should have been thrown.");
    }
    catch(InvalidRequestType& e)
    {
        QVERIFY2(stream.device()->bytesAvailable() == 0,
                 qPrintable(QString("Incorrect number of bytes left in stream. "\
                                    "Is %1.")
                            .arg(stream.device()->bytesAvailable())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readType_NoEx_valid_data()
{
    readType_valid_data();
}

void NetworkProtocolUtilitiesTest::readType_NoEx_invalid_data()
{
    readType_invalid_data();
}

void NetworkProtocolUtilitiesTest::readType_NoEx_valid()
{
    QFETCH(Message::MessageType, message_type);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << message_type;

    stream.device()->reset();
    try
    {
        assert(stream.device()->bytesAvailable() == sizeof(Message::MessageType));
        Message::MessageLengthType is_type = Requests::readType_NoEx(stream);

        QVERIFY2(is_type  == message_type,
                 qPrintable(QString("Message type is incorrect. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(message_type)));
        QVERIFY2(stream.device()->bytesAvailable() == 0,
                 qPrintable(QString("Incorrect number of bytes left in stream. "\
                                    "Is %1.")
                            .arg(stream.device()->bytesAvailable())));
    }
    catch(InvalidRequestType& e)
    {
        QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readType_NoEx_invalid_empty()
{
    QByteArray buffer0;
    QDataStream stream(&buffer0, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);
    assert(stream.device()->bytesAvailable() == 0);
    try
    {
        Message::MessageType type = Requests::readType_NoEx(stream);
        QVERIFY2(type == Message::MESSAGE_UNDEFINED,
                 qPrintable(QString("Type is incorrect. Is %1 should be %2.")
                            .arg(type)
                            .arg(Message::MESSAGE_UNDEFINED)));
    }
    catch(InvalidRequestType& e)
    {
        QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::readType_NoEx_invalid()
{
    QFETCH(Message::MessageTypeType, message_type);

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << message_type;

    stream.device()->reset();
    try
    {
        assert(stream.device()->bytesAvailable() == sizeof(message_type));
        Message::MessageType type = Requests::readType_NoEx(stream);
        QVERIFY2(type == Message::MESSAGE_UNDEFINED,
                 qPrintable(QString("Type is incorrect. Is %1 should be %2.")
                            .arg(type)
                            .arg(Message::MESSAGE_UNDEFINED)));
        QVERIFY2(stream.device()->bytesAvailable() == 0,
                 qPrintable(QString("Incorrect number of bytes left in stream. "\
                                    "Is %1.")
                            .arg(stream.device()->bytesAvailable())));
    }
    catch(InvalidRequestType& e)
    {
        QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::fromStream_valid_data()
{

}


void NetworkProtocolUtilitiesTest::fromStream_invalid_data()
{

}

void NetworkProtocolUtilitiesTest::fromStream_valid()
{
    QSKIP("Not implemented yet.");

}

void NetworkProtocolUtilitiesTest::fromStream_invalid()
{
    QSKIP("Not implemented yet.");

}
