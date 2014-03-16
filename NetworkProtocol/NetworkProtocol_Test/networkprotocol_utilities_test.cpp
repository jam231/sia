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
                                        make_logger(LoggingLevel::Trace)));
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
        Requests::readLength(stream);

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

     QTest::addColumn<Message::MessageType>("message_type");
     QTest::addColumn<QByteArray>("binary_data");

     QString password = "abcde";

     QByteArray buffer1;
     QDataStream stream1(&buffer1, QIODevice::ReadWrite);
     stream1 << static_cast<Message::MessageLengthType>(2 + 1 + 2 + password.toUtf8().size())
            << Message::REQUEST_REGISTER_USER
            << static_cast<Message::MessageLengthType>(password.toUtf8().size());
     stream1.writeRawData(password.toUtf8().data(), password.toUtf8().size());
     QTest::newRow("REQUEST_REGISTER_USER") << Message::REQUEST_REGISTER_USER
                                            << buffer1;

     QByteArray buffer2;
     QDataStream stream2(&buffer2, QIODevice::ReadWrite);
     stream2 << static_cast<Message::MessageLengthType>(2 + 1 + 4 + 2 + password.toUtf8().size())
            << Message::REQUEST_LOGIN_USER
            << UserIdType(1)
            << static_cast<Message::MessageLengthType>(password.toUtf8().size());
     stream2.writeRawData(password.toUtf8().data(), password.toUtf8().size());
     QTest::newRow("REQUEST_LOGIN_USER")
             << Message::REQUEST_LOGIN_USER
             << buffer2;

     QByteArray buffer3;
     QDataStream stream3(&buffer3, QIODevice::ReadWrite);
     stream3 << static_cast<Message::MessageLengthType>(15)
            << Message::REQUEST_SELL_STOCK_ORDER << StockIdType(1)
            << AmountType(1) << PriceType(1);
     QTest::newRow("REQUEST_SELL_STOCK_ORDER")
             << Message::REQUEST_SELL_STOCK_ORDER
             << buffer3;

     QByteArray buffer4;
     QDataStream stream4(&buffer4, QIODevice::ReadWrite);
     stream4 << static_cast<Message::MessageLengthType>(15)
            << Message::REQUEST_BUY_STOCK_ORDER << StockIdType(1) << AmountType(1)
            << PriceType(1);
     QTest::newRow("REQUEST_BUY_STOCK_ORDER")
             << Message::REQUEST_BUY_STOCK_ORDER
             << buffer4;

     QByteArray buffer5;
     QDataStream stream5(&buffer5, QIODevice::ReadWrite);
     stream5 << static_cast<Message::MessageLengthType>(7)
            << Message::REQUEST_SUBSCRIBE_STOCK << StockIdType(1);
     QTest::newRow("REQUEST_SUBSCRIBE_STOCK")
             << Message::REQUEST_SUBSCRIBE_STOCK
             << buffer5;

     QByteArray buffer6;
     QDataStream stream6(&buffer6, QIODevice::ReadWrite);
     stream6 << static_cast<Message::MessageLengthType>(7)
            << Message::REQUEST_UNSUBSCRIBE_STOCK << StockIdType(1);
     QTest::newRow("REQUEST_UNSUBSCRIBE_STOCK")
             << Message::REQUEST_UNSUBSCRIBE_STOCK
             << buffer6;

     QByteArray buffer7;
     QDataStream stream7(&buffer7, QIODevice::ReadWrite);
     stream7 << static_cast<Message::MessageLengthType>(3)
            << Message::REQUEST_GET_MY_STOCKS;
     QTest::newRow("REQUEST_GET_MY_STOCKS")
             << Message::REQUEST_GET_MY_STOCKS
             << buffer7;

     QByteArray buffer8;
     QDataStream stream8(&buffer8, QIODevice::ReadWrite);
     stream8 << static_cast<Message::MessageLengthType>(3)
            << Message::REQUEST_GET_MY_ORDERS;
     QTest::newRow("REQUEST_GET_MY_ORDERS")
             << Message::REQUEST_GET_MY_ORDERS
             << buffer8;

     QByteArray buffer9;
     QDataStream stream9(&buffer9, QIODevice::ReadWrite);
     stream9 << static_cast<Message::MessageLengthType>(7)
            << Message::REQUEST_GET_STOCK_INFO << StockIdType(1);
     QTest::newRow("REQUEST_GET_STOCK_INFO")
             << Message::REQUEST_GET_STOCK_INFO
             << buffer9;

     QByteArray buffer10;
     QDataStream stream10(&buffer10, QIODevice::ReadWrite);
     stream10 << static_cast<Message::MessageLengthType>(7)
            << Message::REQUEST_CANCEL_ORDER << OrderIdType(1);
     QTest::newRow("REQUEST_CANCEL_ORDER")
             << Message::REQUEST_CANCEL_ORDER
             << buffer10;
}


void NetworkProtocolUtilitiesTest::fromStream_invalid_incomplete_data()
{

    // Incomplete requests

    QTest::addColumn<QByteArray>("binary_data");

    QString password = "abcde";

    QByteArray buffer1;
    QDataStream stream1(&buffer1, QIODevice::ReadWrite);
    stream1 << static_cast<Message::MessageLengthType>(2 + 1 + 2 + password.toUtf8().size() + 1) // missing one letter
            << Message::REQUEST_REGISTER_USER
            << static_cast<Message::MessageLengthType>(password.toUtf8().size() + 1);
    stream1.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_REGISTER_USER")
            << buffer1;

    QByteArray buffer2;
    QDataStream stream2(&buffer2, QIODevice::ReadWrite);
    stream2 << static_cast<Message::MessageLengthType>(2 + 1 + 4 + 2 + password.toUtf8().size() + 1) // missing one letter
           << Message::REQUEST_LOGIN_USER
           << UserIdType(1)
           << static_cast<Message::MessageLengthType>(password.toUtf8().size() + 1);
    stream1.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_LOGIN_USER")
            << buffer2;

    QByteArray buffer3;
    QDataStream stream3(&buffer3, QIODevice::ReadWrite);
    stream3 << static_cast<Message::MessageLengthType>(15)
           << Message::REQUEST_SELL_STOCK_ORDER << StockIdType(1)
           << AmountType(1); // Missing price << PriceType(1);
    QTest::newRow("REQUEST_SELL_STOCK_ORDER")
            << buffer3;

    QByteArray buffer4;
    QDataStream stream4(&buffer4, QIODevice::ReadWrite);
    stream4 << static_cast<Message::MessageLengthType>(15)
           << Message::REQUEST_BUY_STOCK_ORDER << StockIdType(1)
           << AmountType(1); // Missing price << PriceType(1);
    QTest::newRow("REQUEST_BUY_STOCK_ORDER")
            << buffer4;

    QByteArray buffer5;
    QDataStream stream5(&buffer5, QIODevice::ReadWrite);
    stream5 << static_cast<Message::MessageLengthType>(7)
            << Message::REQUEST_SUBSCRIBE_STOCK << quint16(1); //<< StockIdType(1);
    QTest::newRow("REQUEST_SUBSCRIBE_STOCK")
            << buffer5;

    QByteArray buffer6;
    QDataStream stream6(&buffer6, QIODevice::ReadWrite);
    stream6 << static_cast<Message::MessageLengthType>(7)
           << Message::REQUEST_UNSUBSCRIBE_STOCK << quint16(1); //<< StockIdType(1);
    QTest::newRow("REQUEST_UNSUBSCRIBE_STOCK")
            << buffer6;

    QByteArray buffer7;
    QDataStream stream7(&buffer7, QIODevice::ReadWrite);
    stream7 << static_cast<Message::MessageLengthType>(3);
           //<< Message::REQUEST_GET_MY_STOCKS;
    QTest::newRow("REQUEST_GET_MY_STOCKS")
            << buffer7;

    QByteArray buffer8;
    QDataStream stream8(&buffer8, QIODevice::ReadWrite);
    stream8 << static_cast<Message::MessageLengthType>(3);
           //<< Message::REQUEST_GET_MY_ORDERS;
    QTest::newRow("REQUEST_GET_MY_ORDERS")
            << buffer8;

    QByteArray buffer9;
    QDataStream stream9(&buffer9, QIODevice::ReadWrite);
    stream9 << static_cast<Message::MessageLengthType>(7)
            << Message::REQUEST_GET_STOCK_INFO << quint16(1) << quint8(0); //<< StockIdType(1);
    QTest::newRow("REQUEST_GET_STOCK_INFO")
            << buffer9;

    QByteArray buffer10;
    QDataStream stream10(&buffer10, QIODevice::ReadWrite);
    stream10 << static_cast<Message::MessageLengthType>(7)
           << Message::REQUEST_CANCEL_ORDER << quint8(0); //<< OrderIdType(1);
    QTest::newRow("REQUEST_CANCEL_ORDER")
            << buffer10;
}

void NetworkProtocolUtilitiesTest::fromStream_valid()
{
    QFETCH(Message::MessageType, message_type);
    QFETCH(QByteArray, binary_data);

    QDataStream stream(&binary_data, QIODevice::ReadOnly);


    stream.device()->reset();
    try
    {
        auto request = fromStream(stream);



        QVERIFY2(request->type() == message_type,
                 qPrintable(QString("Type is incorrect. Is %1 should be %2.")
                            .arg(request->type())
                            .arg(message_type)));

        QVERIFY2(stream.device()->bytesAvailable() == 0,
                 qPrintable(QString("Incorrect number of bytes left in stream. "\
                                    "Is %1.")
                            .arg(stream.device()->bytesAvailable())));
    }
    catch(std::exception& e)
    {
        QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown.");
    }
}

void NetworkProtocolUtilitiesTest::fromStream_invalid_incomplete()
{

        QFETCH(QByteArray, binary_data);


        Message::MessageLengthType length = binary_data.size();

        QDataStream stream(&binary_data, QIODevice::ReadOnly);


        stream.device()->reset();
        try
        {
            auto request = fromStream(//std::shared_ptr<AbstractLogger>(
                                          //make_logger(LoggingLevel::Trace),
                                                      //std::shared_ptr<AbstractWriter>(new QDebugWriter()))),
                                          stream);

        }
        catch(IncompleteRequest&)
        {
            QVERIFY2(stream.device()->bytesAvailable() == length,
                     qPrintable(QString("Incorrect number of bytes left in stream. "\
                                        "Is %1 should be %2.")
                                .arg(stream.device()->bytesAvailable())
                                .arg(length)));
        }
        catch(std::exception& e)
        {
            QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
        }
        catch(...)
        {
            QFAIL("Unknown exception has been thrown.");
        }
}

void NetworkProtocolUtilitiesTest::fromStream_invalid_body_data()
{

    // Invalid request body

    QTest::addColumn<QByteArray>("binary_data");

    QString password = "abc";

    QByteArray buffer1;
    QDataStream stream1(&buffer1, QIODevice::ReadWrite);
    stream1 << static_cast<Message::MessageLengthType>(2 + 1 + 2 + password.toUtf8().size())
           << Message::REQUEST_REGISTER_USER
           << static_cast<Message::MessageLengthType>(password.toUtf8().size());
    stream1.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_REGISTER_USER")
            << buffer1;

    QByteArray buffer2;
    QDataStream stream2(&buffer2, QIODevice::ReadWrite);
    stream2 << static_cast<Message::MessageLengthType>(2 + 1 + 4 + 2 + password.toUtf8().size())
           << Message::REQUEST_LOGIN_USER
           << UserIdType(1)
           << static_cast<Message::MessageLengthType>(password.toUtf8().size());
    stream2.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_LOGIN_USER")
            << buffer2;

    QByteArray buffer2_2;
    QDataStream stream2_2(&buffer2_2, QIODevice::ReadWrite);
    stream2_2
           << static_cast<Message::MessageLengthType>(2 + 1 + 4 + 2 + password.toUtf8().size())
           << Message::REQUEST_LOGIN_USER
           << UserIdType(0)
           << static_cast<Message::MessageLengthType>(password.toUtf8().size());
    stream2_2.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_LOGIN_USER")
            << buffer2_2;

    QByteArray buffer3;
    QDataStream stream3(&buffer3, QIODevice::ReadWrite);
    stream3 << static_cast<Message::MessageLengthType>(15)
           << Message::REQUEST_SELL_STOCK_ORDER << StockIdType(0)
           << AmountType(0) << PriceType(0);
    QTest::newRow("REQUEST_SELL_STOCK_ORDER")
            << buffer3;

    QByteArray buffer4;
    QDataStream stream4(&buffer4, QIODevice::ReadWrite);
    stream4 << static_cast<Message::MessageLengthType>(15)
           << Message::REQUEST_BUY_STOCK_ORDER << StockIdType(0)
           << AmountType(0)
           << PriceType(0);
    QTest::newRow("REQUEST_BUY_STOCK_ORDER")
            << buffer4;

    QByteArray buffer5;
    QDataStream stream5(&buffer5, QIODevice::ReadWrite);
    stream5 << static_cast<Message::MessageLengthType>(7)
           << Message::REQUEST_SUBSCRIBE_STOCK << StockIdType(0);
    QTest::newRow("REQUEST_SUBSCRIBE_STOCK")
            << buffer5;

    QByteArray buffer6;
    QDataStream stream6(&buffer6, QIODevice::ReadWrite);
    stream6 << static_cast<Message::MessageLengthType>(7)
           << Message::REQUEST_UNSUBSCRIBE_STOCK << StockIdType(0);
    QTest::newRow("REQUEST_UNSUBSCRIBE_STOCK")
            << buffer6;

    QByteArray buffer7;
    QDataStream stream7(&buffer7, QIODevice::ReadWrite);
    stream7 << static_cast<Message::MessageLengthType>(3)
            << Message::REQUEST_GET_MY_STOCKS;
    QTest::newRow("REQUEST_GET_MY_STOCKS")
            << buffer7;

    QByteArray buffer8;
    QDataStream stream8(&buffer8, QIODevice::ReadWrite);
    stream8 << static_cast<Message::MessageLengthType>(3)
           << Message::REQUEST_GET_MY_ORDERS;
    QTest::newRow("REQUEST_GET_MY_ORDERS")
            << buffer8;

    QByteArray buffer9;
    QDataStream stream9(&buffer9, QIODevice::ReadWrite);
    stream9 << static_cast<Message::MessageLengthType>(7)
           << Message::REQUEST_GET_STOCK_INFO << StockIdType(0);
    QTest::newRow("REQUEST_GET_STOCK_INFO")
            << buffer9;

    QByteArray buffer10;
    QDataStream stream10(&buffer10, QIODevice::ReadWrite);
    stream10 << static_cast<Message::MessageLengthType>(7)
           << Message::REQUEST_CANCEL_ORDER << OrderIdType(0);
    QTest::newRow("REQUEST_CANCEL_ORDER")
            << buffer10;
}

void NetworkProtocolUtilitiesTest::fromStream_invalid_body()
{

        QFETCH(QByteArray, binary_data);

        QDataStream stream(&binary_data, QIODevice::ReadOnly);


        stream.device()->reset();
        try
        {
            auto request = fromStream(/*std::shared_ptr<AbstractLogger>(
                                          make_logger(LoggingLevel::Trace,
                                                      std::shared_ptr<AbstractWriter>(new QDebugWriter()))),
                                          */stream);

        }
        catch(InvalidRequestBody&)
        {
            QVERIFY2(stream.device()->bytesAvailable() == 0,
                     qPrintable(QString("Incorrect number of bytes left in stream. "\
                                        "Is %1 should be %2.")
                                .arg(stream.device()->bytesAvailable())
                                .arg(0)));
        }
        catch(std::exception& e)
        {
            QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
        }
        catch(...)
        {
            QFAIL("Unknown exception has been thrown.");
        }
}

void NetworkProtocolUtilitiesTest::fromStream_malformed_data()
{

    // Invalid request body

    QTest::addColumn<QByteArray>("binary_data");
    QTest::addColumn<Message::MessageLengthType>("data_size_left");

    QString password = "abcde";

    QByteArray buffer1;
    QDataStream stream1(&buffer1, QIODevice::ReadWrite);
    stream1 << static_cast<Message::MessageLengthType>(2 + 1 + 2 + password.toUtf8().size())
           << Message::REQUEST_REGISTER_USER
           << static_cast<Message::MessageLengthType>(password.toUtf8().size() + 1);
    stream1.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_REGISTER_USER")
            << buffer1
            << static_cast<Message::MessageLengthType>(0);

    QByteArray buffer2;
    QDataStream stream2(&buffer2, QIODevice::ReadWrite);
    stream2 << static_cast<Message::MessageLengthType>(2 + 1 + 4 + 2 + password.toUtf8().size())
           << Message::REQUEST_LOGIN_USER
           << UserIdType(1)
           << static_cast<Message::MessageLengthType>(password.toUtf8().size() + 1);
    stream2.writeRawData(password.toUtf8().data(), password.toUtf8().size());
    QTest::newRow("REQUEST_LOGIN_USER")
            << buffer2
            << static_cast<Message::MessageLengthType>(0);

    QByteArray buffer3;
    QDataStream stream3(&buffer3, QIODevice::ReadWrite);
    stream3 << static_cast<Message::MessageLengthType>(13)
           << Message::REQUEST_SELL_STOCK_ORDER << StockIdType(1)
           << AmountType(1) << PriceType(1);
    QTest::newRow("REQUEST_SELL_STOCK_ORDER")
            << buffer3
            << static_cast<Message::MessageLengthType>(2);

    QByteArray buffer4;
    QDataStream stream4(&buffer4, QIODevice::ReadWrite);
    stream4 << static_cast<Message::MessageLengthType>(11)
           << Message::REQUEST_BUY_STOCK_ORDER << StockIdType(1)
           << AmountType(1)
           << PriceType(1);
    QTest::newRow("REQUEST_BUY_STOCK_ORDER")
            << buffer4
            << static_cast<Message::MessageLengthType>(4);

    QByteArray buffer5;
    QDataStream stream5(&buffer5, QIODevice::ReadWrite);
    stream5 << static_cast<Message::MessageLengthType>(5)
           << Message::REQUEST_SUBSCRIBE_STOCK << StockIdType(1);
    QTest::newRow("REQUEST_SUBSCRIBE_STOCK")
            << buffer5
            << static_cast<Message::MessageLengthType>(2);

    QByteArray buffer6;
    QDataStream stream6(&buffer6, QIODevice::ReadWrite);
    stream6 << static_cast<Message::MessageLengthType>(4)
           << Message::REQUEST_UNSUBSCRIBE_STOCK << StockIdType(1);
    QTest::newRow("REQUEST_UNSUBSCRIBE_STOCK")
            << buffer6
            << static_cast<Message::MessageLengthType>(3);

    QByteArray buffer9;
    QDataStream stream9(&buffer9, QIODevice::ReadWrite);
    stream9 << static_cast<Message::MessageLengthType>(4)
           << Message::REQUEST_GET_STOCK_INFO << StockIdType(2);
    QTest::newRow("REQUEST_GET_STOCK_INFO")
            << buffer9
            << static_cast<Message::MessageLengthType>(3);

    QByteArray buffer10;
    QDataStream stream10(&buffer10, QIODevice::ReadWrite);
    stream10 << static_cast<Message::MessageLengthType>(4)
           << Message::REQUEST_CANCEL_ORDER << OrderIdType(1);
    QTest::newRow("REQUEST_CANCEL_ORDER")
            << buffer10
            << static_cast<Message::MessageLengthType>(3);

}

void NetworkProtocolUtilitiesTest::fromStream_malformed()
{

        QFETCH(QByteArray, binary_data);
        QFETCH(Message::MessageLengthType, data_size_left);

        QDataStream stream(&binary_data, QIODevice::ReadOnly);

        stream.device()->reset();
        try
        {
            fromStream(std::shared_ptr<AbstractLogger>(
                                          make_logger(LoggingLevel::Trace,
                                                      std::shared_ptr<AbstractWriter>(new QDebugWriter()))),
                                          stream);

        }
        catch(MalformedRequest&)
        {
            QVERIFY2(stream.device()->bytesAvailable() == data_size_left,
                     qPrintable(QString("Incorrect number of bytes left in stream. "\
                                        "Is %1 should be %2.")
                                .arg(stream.device()->bytesAvailable())
                                .arg(data_size_left)));
        }
        catch(std::exception& e)
        {
            QFAIL(qPrintable(QString("Caught: %1").arg(e.what())));
        }
        catch(...)
        {
            QFAIL("Unknown exception has been thrown.");
        }
}
