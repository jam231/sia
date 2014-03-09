#include "registerusersuccessmsg_test.h"

#include <Responses/registerusersuccessmsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(UserIdType)


void RegisterUserSuccessTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}

void RegisterUserSuccessTest::generate_valid_data()
{
    QTest::addColumn<UserIdType>("user_id");


    for(long long i = 2; i <= 32; i += 2)
    {
        UserIdType user_id = UserIdType((static_cast<long long>(1) << i) - 1);
        QTest::newRow(qPrintable(QString("user id = 2 ^ %1 - 1= %2")
                                 .arg(i).arg(user_id.value)))
                << user_id;
    }
}

void RegisterUserSuccessTest::creation_invalid_data()
{
    QTest::addColumn<UserIdType>("user_id");
    QTest::newRow("user id = 0") << UserIdType(0);
}

void RegisterUserSuccessTest::creation_invalid()
{
    QFETCH(UserIdType, user_id);

    try
    {
        Responses::RegisterUserSuccess success(user_id);

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

void RegisterUserSuccessTest::creation_valid_data()
{
    generate_valid_data();
}

void RegisterUserSuccessTest::creation_valid()
{
    QFETCH(UserIdType, user_id);

    try
    {
        Responses::RegisterUserSuccess success(user_id);

        QVERIFY2(success.getUserId()  == user_id,  "User id is corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}

void RegisterUserSuccessTest::constant_length_data()
{
    generate_valid_data();
}

void RegisterUserSuccessTest::constant_length()
{
    QFETCH(UserIdType, user_id);

    try
    {
        Responses::RegisterUserSuccess success(user_id);

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(UserIdType);

        QVERIFY2(success.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(success.length())));
        QVERIFY2(success.length() == 7,
                 qPrintable(QString("Message length doesn't match specification."\
                                    "Should be %1 is %2.")
                            .arg(7)
                            .arg(success.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void RegisterUserSuccessTest::send_data()
{
    generate_valid_data();
}

void RegisterUserSuccessTest::send()
{
    QFETCH(UserIdType, user_id);

    try
    {
        Responses::RegisterUserSuccess success(user_id);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        success.send(stream.device());

        stream.device()->reset();

        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                     sizeof(Message::MessageType) +
                                                     sizeof(UserIdType);

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
        UserIdType                 is_user_id;

        stream >> is_length >> is_type >> is_user_id;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_FAILURE == 0x3);

                QVERIFY2(is_type == Message::RESPONSE_REGISTER_USER_SUCCESS,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_REGISTER_USER_SUCCESS)));

        QVERIFY2(is_user_id == user_id,
                 qPrintable(QString("User id doesn't match. Is %1 should be %2.")
                            .arg(is_user_id.value)
                            .arg(user_id.value)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}
