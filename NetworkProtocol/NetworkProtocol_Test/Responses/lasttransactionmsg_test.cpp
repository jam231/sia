#include "lasttransactionmsg_test.h"

#include <Responses/lasttransactionmsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(StockIdType)
Q_DECLARE_METATYPE(AmountType)
Q_DECLARE_METATYPE(PriceType)



void LastTransactionResponseTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}

void LastTransactionResponseTest::generate_valid_data()
{
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");
    QTest::addColumn<QString>("date_time_utf8");

    for(int i = 1; i < 60000; i += 10000)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        QTest::newRow("valid data") << StockIdType(i) << AmountType(i)
                                    << PriceType(i + i % 60) << date_time_str ;
    }
}

void LastTransactionResponseTest::generate_invalid_data()
{
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<AmountType>("amount");
    QTest::addColumn<PriceType>("price");
    QTest::addColumn<QString>("date_time_utf8");

    for(int i = 0; i < 60; i += 10)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();

        QTest::newRow("invalid data") << StockIdType(i) << AmountType(0)
                                      << PriceType(i) << date_time_str;
    }
    for(int i = 0; i < 60; i += 10)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();

        QTest::newRow("invalid data") <<StockIdType(i) << AmountType(i)
                                      << PriceType(0) << date_time_str ;
    }
}

void LastTransactionResponseTest::creation_valid_data()
{
    generate_valid_data();
}
void LastTransactionResponseTest::creation_valid()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);
    QFETCH(QString, date_time_utf8);


    try
    {
        Responses::LastTransaction last_transaction(stock_id, amount, price,
                                                   date_time_utf8);
        QVERIFY2(last_transaction.getStockId()    == stock_id,"Stock id is corrupted.");
        QVERIFY2(last_transaction.getLastTransaction().getDateTime() == date_time_utf8,
                 "DateTime is corruped.");
        QVERIFY2(last_transaction.getLastTransaction().getPrice()      == price,
                 "Stock id is corrupted.");
        QVERIFY2(last_transaction.getLastTransaction().getAmount()     == amount,
                 "Amount is corrupted.");
    }
    catch(...)
    {
        QFAIL("Exception has been thrown");
    }
}




void LastTransactionResponseTest::creation_invalid_data()
{
    generate_invalid_data();
}

void LastTransactionResponseTest::creation_invalid()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);
    QFETCH(QString, date_time_utf8);

    try
    {
        Responses::LastTransaction last_transaction(stock_id, amount, price,
                                                    date_time_utf8);
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


void LastTransactionResponseTest::constant_length_data()
{
    generate_valid_data();
}

void LastTransactionResponseTest::constant_length()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);
    QFETCH(QString, date_time_utf8);

    try
    {
        Responses::LastTransaction last_transaction(stock_id, amount, price,
                                                    date_time_utf8);

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(StockIdType) +
                                                   last_transaction.getLastTransaction().lengthSerialized();
        QVERIFY2(last_transaction.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(last_transaction.length())));
        Message::MessageLengthType should_be_atleast_length =
                                                sizeof(Message::MessageLengthType) +
                                                sizeof(Message::MessageType) +
                                                sizeof(StockIdType);
        QVERIFY2(last_transaction.length() > should_be_atleast_length,
                 qPrintable(QString("Message length should be %1 <= %2( = Is).")
                            .arg(should_be_atleast_length)
                            .arg(last_transaction.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void LastTransactionResponseTest::send_data()
{
    generate_valid_data();
}

void LastTransactionResponseTest::send()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(AmountType, amount);
    QFETCH(PriceType, price);
    QFETCH(QString, date_time_utf8);

    try
    {
        Responses::LastTransaction last_transaction(stock_id, amount, price,
                                                    date_time_utf8);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        last_transaction.send(stream.device());

        stream.device()->reset();


        Message::MessageLengthType should_be_bytes =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(StockIdType) +
                                                   last_transaction.getLastTransaction().lengthSerialized();

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        StockIdType                is_stock_id;

        stream >> is_length >> is_type >> is_stock_id;

        LastTransaction is_last_transaction = LastTransaction::fromStream(stream);

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_LAST_TRANSACTION == 0x18);

                QVERIFY2(is_type == Message::RESPONSE_LAST_TRANSACTION,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_LAST_TRANSACTION)));

        QVERIFY2(is_stock_id == stock_id,
                 qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                            .arg(is_stock_id.value)
                            .arg(stock_id.value)));

        QVERIFY2(is_last_transaction.getDateTime() == date_time_utf8,
                 qPrintable(QString("Date time doesn't match. Is %1 should be %2.")
                            .arg(is_last_transaction.getDateTime())
                            .arg(date_time_utf8)));
        QVERIFY2(is_last_transaction.getAmount()     == amount,
                 qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                            .arg(is_last_transaction.getAmount().value)
                            .arg(amount.value)));
        QVERIFY2(is_last_transaction.getPrice()      == price,
                 qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                            .arg(is_last_transaction.getPrice().value)
                            .arg(price.value)));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}
