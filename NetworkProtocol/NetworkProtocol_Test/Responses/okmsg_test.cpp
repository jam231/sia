#include "okmsg_test.h"

#include <Responses/okmsg.h>

#include <utilities.h>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

void OkTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}


void OkTest::constant_length()
{

    try
    {
        Responses::Ok ok;

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType);

        QVERIFY2(ok.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(ok.length())));
        QVERIFY2(ok.length() == 3,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(3)
                            .arg(ok.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}


void OkTest::send()
{

    try
    {
        Responses::Ok ok;

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        ok.send(stream.device());

        stream.device()->reset();

        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                     sizeof(Message::MessageType);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        QVERIFY2(stream.device()->size() == 3,
                 qPrintable(QString("Bytes saved in device doesn't match with"\
                                    "specification. Should be %1 is %2.")
                            .arg(3)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;

        stream >> is_length >> is_type;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_OK == 0x2);

        QVERIFY2(is_type == Message::RESPONSE_OK,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_OK)));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}
