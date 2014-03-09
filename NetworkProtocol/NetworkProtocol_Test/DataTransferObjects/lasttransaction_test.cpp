#include "lasttransaction_test.h"

#include <DataTransferObjects/types.h>
#include <DataTransferObjects/lasttransaction.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

#include <QDataStream>

using namespace NetworkProtocol::DTO;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)

void LastTransactionTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}
void LastTransactionTest::generate_valid_data()
{
    QTest::addColumn<QString>("date_time_str");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 1; i < 60000; i += 10000)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("valid data") << date_time_str << AmountType(i)
                                    << PriceType(i + i % 60);
    }
}
void LastTransactionTest::generate_invalid_data()
{
    QTest::addColumn<QString>("date_time_str");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");

    for(int i = 0; i < 60; i += 10)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("invalid data") << date_time_str << AmountType(0)
                                      << PriceType(i);
    }
    for(int i = 0; i < 60; i += 10)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("invalid data") << date_time_str << AmountType(i)
                                      << PriceType(0);
    }
}

void LastTransactionTest::creation_valid_data()
{
    generate_valid_data();
}

void LastTransactionTest::creation_valid()
{
    QFETCH(QString, date_time_str);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        LastTransaction last_transaction(amount, price, date_time_str);
        QVERIFY2(last_transaction.getDateTime() == date_time_str,   "DateTime is corruped.");
        QVERIFY2(last_transaction.getPrice()      == price,         "Stock id is corrupted.");
        QVERIFY2(last_transaction.getAmount()     == amount,        "Amount is corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}


void LastTransactionTest::creation_invalid_data()
{
    generate_invalid_data();
}

void LastTransactionTest::creation_invalid()
{
    QFETCH(QString, date_time_str);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        LastTransaction last_transaction(amount, price, date_time_str);
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

void LastTransactionTest::toStream_valid_data()
{
    generate_valid_data();
}

void LastTransactionTest::toStream_valid()
{
    QFETCH(QString, date_time_str);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        LastTransaction last_transaction(amount, price, date_time_str);
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        stream << last_transaction;
        stream.device()->reset();
        qint32 should_be_bytes = sizeof(amount) + sizeof(price) +
                                 sizeof(Message::MessageLengthType) +
                                 date_time_str.toUtf8().size();
        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        Message::MessageLengthType date_time_length;
        AmountType should_be_amount;
        PriceType should_be_price;

        stream >> should_be_amount >> should_be_price;
        stream >> date_time_length;

        QByteArray tmp_buffer;
        tmp_buffer.resize(date_time_length);

        stream.readRawData(tmp_buffer.data(), date_time_length);
        QString should_be_date_time_str = QString::fromUtf8(tmp_buffer);

        QVERIFY2(should_be_date_time_str == date_time_str,
                 qPrintable(QString("DateTime doesn't match. Is %1 should be %2")
                            .arg(should_be_date_time_str)
                            .arg(date_time_str)));
        QVERIFY2(should_be_amount == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2")
                            .arg(should_be_amount.value)
                            .arg(amount.value)));
        QVERIFY2(should_be_price == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2")
                            .arg(should_be_price.value)
                            .arg(price.value)));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }

}


void LastTransactionTest::fromStream_valid_data()
{
    generate_valid_data();
}


void LastTransactionTest::fromStream_valid()
{
    QFETCH(QString, date_time_str);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType date_time_str_length = date_time_str.toUtf8().size();
        stream << amount << price;
        stream << date_time_str_length;
        stream.writeRawData(date_time_str.toUtf8(), date_time_str_length);
        // get ready for reading.
        stream.device()->reset();

        Types::Message::MessageLengthType should_be_bytes = sizeof(date_time_str_length)
                + date_time_str_length + sizeof(amount) + sizeof(price);

        QVERIFY2(stream.device()->bytesAvailable() == should_be_bytes,
                 qPrintable(QString("Bytes available in device doesn't equal to"\
                                    " (supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        LastTransaction from_stream = LastTransaction::fromStream(stream);

        QVERIFY2(from_stream.getDateTime() == date_time_str,
                 qPrintable(QString("DateTime doesn't match. Is %1 should be %2")
                            .arg(from_stream.getDateTime())
                            .arg(date_time_str)));
        QVERIFY2(from_stream.getAmount() == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2")
                            .arg(from_stream.getAmount().value)
                            .arg(amount.value)));
        QVERIFY2(from_stream.getPrice() == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2")
                            .arg(from_stream.getPrice().value)
                            .arg(price.value)));
    }
    catch(std::exception& e)
    {
        QFAIL(qPrintable(QString("Exception has been thrown : %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unkown exception has been thrown.");
    }}

void LastTransactionTest::fromStream_invalid_data()
{
    generate_invalid_data();
}


void LastTransactionTest::fromStream_invalid()
{
    QFETCH(QString, date_time_str);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType date_time_str_length = date_time_str.toUtf8().size();
        stream << amount << price;
        stream << date_time_str_length;
        stream.writeRawData(date_time_str.toUtf8(), date_time_str_length);
        // get ready for reading.
        stream.device()->reset();

        Types::Message::MessageLengthType should_be_bytes = sizeof(date_time_str_length)
                + date_time_str_length + sizeof(amount) + sizeof(price);

        QVERIFY2(stream.device()->bytesAvailable() == should_be_bytes,
                 qPrintable(QString("Bytes available in device doesn't equal to "\
                                    " (supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        LastTransaction::fromStream(stream);
        QFAIL("std::invalid_argument should have been thrown.");

    }
    catch(std::invalid_argument&)
    {
        // This should have been thrown.
        return;
    }
    catch(...)
    {
        QFAIL("Unkown exception has been thrown.");
    }
}

void LastTransactionTest::lengthSerialized_data()
{
    generate_valid_data();
}

void LastTransactionTest::lengthSerialized()
{
    QFETCH(QString, date_time_str);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);

    LastTransaction last_transaction(amount, price, date_time_str);

    Types::Message::MessageLengthType date_time_str_length = date_time_str.toUtf8().size();
    Message::MessageLengthType sum_of_sizeofs = sizeof(date_time_str_length) +
                                                date_time_str_length + sizeof(amount)
                                                + sizeof(price);

    QVERIFY2(last_transaction.lengthSerialized() == sum_of_sizeofs,
             qPrintable(QString("Last transaction length in bytes is incorrect."\
                                "Is %1 should be %2.")
                        .arg(last_transaction.lengthSerialized())
                        .arg(sum_of_sizeofs)));
}

