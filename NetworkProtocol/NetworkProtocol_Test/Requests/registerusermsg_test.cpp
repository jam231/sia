#include "registerusermsg_test.h"

#include <Requests/registerusermsg.h>
#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

#include <cmath>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(Message::MessageLengthType)

void RegisterUserTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void RegisterUserTest::from_valid_data()
{
    QTest::addColumn<QString>("password");

    for(long long i = 14; i <= 32; i += 2)
    {
        QString password = QString::number((static_cast<long long>(1) << i) - 1);

        QTest::newRow(qPrintable(QString("password = %1")
                                 .arg(password)))
                << password;
    }
}

void RegisterUserTest::from_valid()
{
    QFETCH(QString, password);
    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType) +
                sizeof(Message::MessageLengthType) + password.size();

        assert (Message::REQUEST_REGISTER_USER == 0x0);

        stream << static_cast<Message::MessageLengthType>(password.toUtf8().size());
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());

        stream.device()->reset();

        Requests::RegisterUser register_user(stream);

        QVERIFY2(register_user.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(register_user.length())
                            .arg(length)));

        QVERIFY2(register_user.getPassword() == password,
                 qPrintable(QString("Password doesn't match. Is %1 should be %2.")
                            .arg(register_user.getPassword())
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

void RegisterUserTest::from_invalid_body_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    assert(stream.byteOrder() == QDataStream::BigEndian);

    QString password = "";
    stream << static_cast<Message::MessageLengthType>(password.toUtf8().size());
    stream.writeRawData(password.toUtf8(), password.toUtf8().size());

    assert(buffer.size() == 2);
    QTest::newRow(qPrintable(QString("Password = %1").arg("")))
            << buffer;

    for(int i = 1; i < 1000; i *= 10)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);
        QString password = QString::number(i);

        stream << static_cast<Message::MessageLengthType>(password.toUtf8().size());
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());
        QTest::newRow(qPrintable(QString("Password = %1").arg(password)))
                << buffer;
    }
}

void RegisterUserTest::from_invalid_body()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::RegisterUser request(stream);
    }
    catch(Requests::InvalidRequestBodyError&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Wrong exception has been thrown.");
    }
}

void RegisterUserTest::from_malformed_data()
{
    QTest::addColumn<QByteArray>("byte_array");

    for(int i = 10000; i < 10000000; i *= 10)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);
        QString password = QString::number(i);

        stream << static_cast<Message::MessageLengthType>(password.toUtf8().size()
                                                          + std::pow(-1, std::log10(i)));
        stream.writeRawData(password.toUtf8(), password.toUtf8().size());
        QTest::newRow(qPrintable(QString("Password = %1").arg(password)))
                << buffer;
    }
}

void RegisterUserTest::from_malformed()
{
    QFETCH(QByteArray, byte_array);

    try
    {
        QDataStream stream(&byte_array, QIODevice::ReadOnly);
        Requests::RegisterUser request(stream);
    }
    catch(Requests::MalformedRequestError&)
    {
        return;
    }
    catch(...)
    {
        QFAIL("Wrong exception has been thrown.");
    }
}
