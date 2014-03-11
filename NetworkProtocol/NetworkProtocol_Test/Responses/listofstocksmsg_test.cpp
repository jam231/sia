#include "listofstocksmsg_test.h"

#include <Responses/listofstocksmsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol;
using namespace Types;
using namespace Order;

struct MyLittle2
{
	StockIdType stock_id;
	AmountType  amount;
};

Q_DECLARE_METATYPE(MyLittle2)

void ListOfStocksTest::initTestCase()
{
    GlobalUtilities::setLogger(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off)));
}

void ListOfStocksTest::generate_valid_data()
{
    QTest::addColumn<QList<MyLittle2> >("list_of_stocks");

    for(int i = 1; i < 60000; i += 10000)
    {
        QList<MyLittle2> list_of_stocks;
        for(int j = 0; j < 10; j++)
        {
            list_of_stocks << MyLittle2 { StockIdType(i + j), AmountType(i + j) };
        }
        QTest::newRow("valid data") << list_of_stocks;
    }
}


void ListOfStocksTest::length_data()
{
    generate_valid_data();
}


void ListOfStocksTest::length()
{
    QFETCH(QList<MyLittle2>, list_of_stocks);


    try
    {
        Responses::ListOfStocks list_of_stocks_msg;

        for(QList<MyLittle2>::iterator it = list_of_stocks.begin();
            it != list_of_stocks.end(); it++)
        {
            list_of_stocks_msg.addStock(it->stock_id, it->amount);
        }

        Message::MessageLengthType should_be_message_length =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Message::MessageLengthType);
        should_be_message_length += list_of_stocks.size() * (sizeof(StockIdType) +
                                                             sizeof(AmountType));

        QVERIFY2(list_of_stocks_msg.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(list_of_stocks_msg.length())));

        Message::MessageLengthType should_be_length_spec =
                                                   sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Message::MessageLengthType) +
                                                   8 * list_of_stocks.size();
        QVERIFY2(list_of_stocks_msg.length() == should_be_length_spec,
                 qPrintable(QString("Message doesn't match specification. "\
                                    "Should be %1 is %2.")
                            .arg(should_be_length_spec)
                            .arg(list_of_stocks_msg.length())));

    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}



void ListOfStocksTest::send_data()
{
    generate_valid_data();
}

void ListOfStocksTest::send()
{
    QFETCH(QList<MyLittle2>, list_of_stocks);

    try
    {
        Responses::ListOfStocks list_of_stocks_msg;
        QList<MyLittle2> should_be_list_of_stocks = list_of_stocks;
        
        for(QList<MyLittle2>::iterator it = list_of_stocks.begin();
            it != list_of_stocks.end(); it++)
        {
            list_of_stocks_msg.addStock(it->stock_id , it->amount);
        }

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        list_of_stocks_msg.send(stream.device());

        stream.device()->reset();


        Message::MessageLengthType should_be_bytes = sizeof(Message::MessageLengthType) +
                                                   sizeof(Message::MessageType) +
                                                   sizeof(Message::MessageLengthType);
        should_be_bytes += list_of_stocks.size() * (sizeof(StockIdType) +
                                                    sizeof(AmountType));

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        Message::MessageLengthType          is_length;
        Message::MessageType                is_type;
        Message::MessageLengthType          is_stocks_count;
        QList<MyLittle2>                    is_list_of_stocks;

        stream >> is_length >> is_type >> is_stocks_count;
        for(int i = 0; i < is_stocks_count; i++)
        {
        	StockIdType stock_id;
        	AmountType  amount;
            stream >> stock_id >> amount;
            is_list_of_stocks << MyLittle2 { stock_id, amount};
		}
        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));

        assert (Message::RESPONSE_LIST_OF_STOCKS == 0x1E);

        QVERIFY2(is_type == Message::RESPONSE_LIST_OF_STOCKS,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_LIST_OF_STOCKS)));

        QVERIFY2(is_stocks_count == list_of_stocks.size(),
                 qPrintable(QString("Stocks count match. Is %1 should be %2.")
                                    .arg(is_stocks_count)
                                    .arg(list_of_stocks.size())));

        for(int i = 0; i < is_stocks_count; i++)
        {
            QVERIFY2(should_be_list_of_stocks[i].stock_id == is_list_of_stocks[i].stock_id,
                     qPrintable(QString("Stock id doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_stocks[i].stock_id.value)
                            .arg(should_be_list_of_stocks[i].stock_id.value)
                            .arg(i)));

            QVERIFY2(should_be_list_of_stocks[i].amount == is_list_of_stocks[i].amount,
                     qPrintable(QString("Stock id doesn't match on position %3. "\
                                        "Is %1 should be %2.")
                            .arg(is_list_of_stocks[i].amount.value)
                            .arg(should_be_list_of_stocks[i].amount.value)
                            .arg(i)));
        }
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

