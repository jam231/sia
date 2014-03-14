#include "loginusermsg_test.h"

#include <Requests/loginusermsg.h>
#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(UserIdType)
Q_DECLARE_METATYPE(Message::MessageLengthType)

void LoginUserTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void LoginUserTest::from_valid_data()
{
    QTest::addColumn<UserIdType>("user_id");
    QTest::addColumn<QString>("password");

    for(long long i = 16; i <= 32; i += 2)
    {
        long long tmp = (static_cast<long long>(1) << i) - 1;
        QString password = QString::number(tmp);
        UserIdType user_id = UserIdType(tmp);

        QTest::newRow(qPrintable(QString("user_id = %1, password = %2")
                                 .arg(user_id.value)
                                 .arg(password)))
                << user_id << password;
    }
}

void LoginUserTest::from_valid()
{
    QFETCH(UserIdType, user_id);
    QFETCH(QString, password);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType) +
                sizeof(UserIdType) +
                sizeof(Message::MessageLengthType) + password.size();

        assert (Message::REQUEST_LOGIN_USER == 0x4);

        stream << user_id;
        stream << static_cast<Message::MessageLengthType>(password.toUtf8().size());
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());

        stream.device()->reset();

        Requests::LoginUser login_user(stream);

        QVERIFY2(login_user.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(login_user.length())
                            .arg(length)));
        QVERIFY2(login_user.getUserId() == user_id,
                 qPrintable(QString("User id doesn't match. Is %1 should be %2.")
                            .arg(login_user.getUserId().value)
                            .arg(user_id.value)));
        QVERIFY2(login_user.getUserPassword() == password,
                 qPrintable(QString("Password doesn't match. Is %1 should be %2.")
                            .arg(login_user.getUserPassword())
                            .arg(password)));

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

void LoginUserTest::from_invalid_body_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);

    QString password = "";
    stream << UserIdType(1);
    stream << static_cast<Message::MessageLengthType>(password.toUtf8().size());
    stream.writeRawData(password.toUtf8(), password.toUtf8().size());

    assert(buffer.size() == 6);
    QTest::newRow(qPrintable(QString("Password = %1").arg("")))
            << buffer;

    for(int i = 1; i < 10000; i *= 10)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);
        QString password = QString::number(i);
        stream << UserIdType(i % 1000);
        stream << static_cast<Message::MessageLengthType>(password.toUtf8().size());
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());
        QTest::newRow(qPrintable(QString("Password = %1").arg(password)))
                << buffer;
    }
}

void LoginUserTest::from_invalid_body()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::LoginUser request(stream);
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

#include <cmath>


void LoginUserTest::from_malformed_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    for(int i = 10000; i < 10000000; i *= 10)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);
        QString password = QString::number(i);

        stream << static_cast<Message::MessageLengthType>((password.toUtf8().size() + 1) % 6);
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());
        QTest::newRow(qPrintable(QString("Password = %1").arg(password)))
                << buffer;
    }
    // With valid user_id
    for(int i = 10000; i < 10000000; i *= 10)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);
        QString password = QString::number(i);
        stream << UserIdType(i);
        stream << static_cast<Message::MessageLengthType>(password.toUtf8().size()
                                                          + std::pow(-1, std::log10(i)));
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());
        QTest::newRow(qPrintable(QString("Password = %1").arg(password)))
                << buffer;
    }
}

void LoginUserTest::from_malformed()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::LoginUser request(stream);
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
