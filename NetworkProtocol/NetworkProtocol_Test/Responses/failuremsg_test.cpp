#include "failuremsg_test.h"


#include <DataTransferObjects/types.h>
#include <Responses/failuremsg.h>

#include <utilities.h>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

Q_DECLARE_METATYPE(Failure::FailureType)

void FailureTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void FailureTest::generate_failure_types()
{
    QTest::addColumn<Failure::FailureType>("failure_type");
    QTest::newRow("BAD_USERID_OR_PASSWORD")  << Failure::BAD_USERID_OR_PASSWORD;
    QTest::newRow("NOT_AUTHORIZED")          << Failure::NOT_AUTHORIZED;
    QTest::newRow("ALREADY_LOGGED")          << Failure::ALREADY_LOGGED;
    QTest::newRow("RESOURCE_NOT_AVAILABLE")  << Failure::RESOURCE_NOT_AVAILABLE;
    QTest::newRow("INSUFFICIENT_FUNDS")      << Failure::INSUFFICIENT_FUNDS;
    QTest::newRow("MALFORMED_MESSAGE")       << Failure::MALFORMED_MESSAGE;
    QTest::newRow("INVALID_MESSAGE_BODY")    << Failure::INVALID_MESSAGE_BODY;
    QTest::newRow("UNRECOGNIZED_MESSAGE")    << Failure::UNRECOGNIZED_MESSAGE;
}



void FailureTest::constant_length_data()
{
    generate_failure_types();
}

void FailureTest::constant_length()
{
    QFETCH(Failure::FailureType, failure_type);

    try
    {
        Responses::Failure fail(failure_type);

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Failure::FailureType);

        QVERIFY2(fail.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(fail.length())));
        QVERIFY2(fail.length() == 4,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(4)
                            .arg(fail.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void FailureTest::send_data()
{
    generate_failure_types();
}

void FailureTest::send()
{
    QFETCH(Failure::FailureType, failure_type);

    try
    {
        Responses::Failure fail(failure_type);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        fail.send(stream.device());

        stream.device()->reset();

        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                     sizeof(Message::MessageType) +
                                                     sizeof(Failure::FailureType);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        QVERIFY2(stream.device()->size() == 4,
                 qPrintable(QString("Bytes saved in device doesn't match with"\
                                    "specification. Should be %1 is %2.")
                            .arg(4)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        Failure::FailureType       is_failure_type;

        stream >> is_length >> is_type >> is_failure_type;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_FAILURE == 0x3);

                QVERIFY2(is_type == Message::RESPONSE_FAILURE,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_FAILURE)));

        QVERIFY2(is_failure_type == failure_type,
                 qPrintable(QString("Failure type doesn't match. Is %1 should be %2.")
                            .arg(is_failure_type)
                            .arg(failure_type)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

