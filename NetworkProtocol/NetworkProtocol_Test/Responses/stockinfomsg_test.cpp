#include "stockinfomsg_test.h"

#include <Responses/stockinfomsg.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol;
using namespace Types;
using namespace Order;

Q_DECLARE_METATYPE(StockIdType)

Q_DECLARE_METATYPE(std::shared_ptr<LastTransaction>)
Q_DECLARE_METATYPE(std::shared_ptr<BestOrder>)

void StockInfoTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                        make_logger(LoggingLevel::Off))));
}


void StockInfoTest::generate_valid_data()
{
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<std::shared_ptr<BestOrder> >("best_buy_order");
    QTest::addColumn<std::shared_ptr<BestOrder> >("best_sell_order");
    QTest::addColumn<std::shared_ptr<LastTransaction> >("last_transaction");

    for(int i = 0; i < 60; i += 10)
    {
        QString date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        StockIdType stock_id = i + 1;
        auto last_transaction = std::shared_ptr<LastTransaction>(
                    new DTO::LastTransaction(i + 1, i + 1, date_time_str));

        auto buy_best_order = std::shared_ptr<BestOrder>(
                    new BestOrder(Order::BUY, stock_id, i + 1, i + 1));

        auto sell_best_order = std::shared_ptr<BestOrder>(
                    new BestOrder(Order::SELL, stock_id, i + 1, i + 1));


        QTest::newRow("valid data (full)")
                << stock_id << buy_best_order
                << sell_best_order << last_transaction;
        QTest::newRow("valid data (buy is null)")
                << stock_id << std::shared_ptr<BestOrder>()
                << sell_best_order << last_transaction;
        QTest::newRow("valid data (sell is null)")
                << stock_id << buy_best_order
                << std::shared_ptr<BestOrder>() << last_transaction;
        QTest::newRow("valid data (last transction is null)")
                << stock_id << buy_best_order
                << sell_best_order << std::shared_ptr<LastTransaction>();
    }
}

void StockInfoTest::generate_invalid_data()
{
    QTest::addColumn<StockIdType>("stock_id");
    QTest::addColumn<std::shared_ptr<BestOrder> >("best_buy_order");
    QTest::addColumn<std::shared_ptr<BestOrder> >("best_sell_order");
    QTest::addColumn<std::shared_ptr<LastTransaction> >("last_transaction");

    QString date_time_str;
    StockIdType                      stock_id;
    std::shared_ptr<LastTransaction> last_transaction;
    std::shared_ptr<BestOrder>       buy_best_order;
    std::shared_ptr<BestOrder>       sell_best_order;

    for(int i = 0; i < 60; i += 10)
    {
        // Stock id = 0 or Stock id doesn't match with last transaction or best orders.
        date_time_str = QDateTime::currentDateTime()
                                        .addDays(-((i / 10000) + 1)).toString();
        stock_id = i;
        last_transaction = std::shared_ptr<LastTransaction>(
                    new DTO::LastTransaction(i + 1 , i + 1, date_time_str));

        buy_best_order = std::shared_ptr<BestOrder>(
                    new BestOrder(Order::BUY, i + 1 , i + 1, i + 1));

        sell_best_order = std::shared_ptr<BestOrder>(
                    new BestOrder(Order::SELL, i + 1 , i + 1, i + 1));


        QTest::newRow("invalid data") << stock_id << buy_best_order
                                      << sell_best_order << last_transaction;
    }

    // Wrong order of best orders.
    last_transaction = std::shared_ptr<LastTransaction>(
                new DTO::LastTransaction(1, 1, date_time_str));

    buy_best_order = std::shared_ptr<BestOrder>(
                new BestOrder(Order::BUY, stock_id, 1, 1));

    sell_best_order = std::shared_ptr<BestOrder>(
                new BestOrder(Order::SELL, stock_id, 1, 1));

    QTest::newRow("invalid data") << stock_id << sell_best_order
                                  << buy_best_order << last_transaction;
}

void StockInfoTest::creation_valid_data()
{
    generate_valid_data();
}
void StockInfoTest::creation_valid()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(std::shared_ptr<BestOrder>, best_buy_order);
    QFETCH(std::shared_ptr<BestOrder>, best_sell_order);
    QFETCH(std::shared_ptr<LastTransaction>, last_transaction);


    try
    {
        Responses::StockInfo stock_info(stock_id, best_buy_order, best_sell_order,
                                        last_transaction);

        QVERIFY2(stock_info.getStockId()                         == stock_id,
                 "Stock id is corrupted.");
        if(best_buy_order)
        {
            QVERIFY2(stock_info.getBestBuyOrder()->getPrice()        == best_buy_order->getPrice(),
                     "Best buy order is corrupted.");
            QVERIFY2(stock_info.getBestBuyOrder()->getAmount()       == best_buy_order->getAmount(),
                     "Best buy order is corrupted.");
        }
        if(best_sell_order)
        {
            QVERIFY2(stock_info.getBestSellOrder()->getPrice()       == best_sell_order->getPrice(),
                     "Best sell order is corrupted.");
            QVERIFY2(stock_info.getBestSellOrder()->getAmount()      == best_sell_order->getAmount(),
                     "Best sell order amount is corrupted.");
        }
        if(last_transaction)
        {
            QVERIFY2(stock_info.getLastTransaction()->getDateTime()  == last_transaction->getDateTime(),
                     "LastTransaction date time is corruped.");
            QVERIFY2(stock_info.getLastTransaction()->getPrice()     == last_transaction->getPrice(),
                     "LastTransaction stock id is corrupted.");
            QVERIFY2(stock_info.getLastTransaction()->getAmount()    == last_transaction->getAmount(),
                     "LastTransaction amount is corrupted.");
        }
    }
    catch(std::invalid_argument e)
    {
        QFAIL(qPrintable(QString("Caught %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Unknown exception has been thrown");
    }
}


void StockInfoTest::creation_invalid_data()
{
    generate_invalid_data();
}

void StockInfoTest::creation_invalid()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(std::shared_ptr<BestOrder>, best_buy_order);
    QFETCH(std::shared_ptr<BestOrder>, best_sell_order);
    QFETCH(std::shared_ptr<LastTransaction>, last_transaction);

    try
    {
        Responses::StockInfo stock_info(stock_id, best_buy_order, best_sell_order,
                                        last_transaction);
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


void StockInfoTest::length_data()
{
    generate_valid_data();
}

void StockInfoTest::length()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(std::shared_ptr<BestOrder>, best_buy_order);
    QFETCH(std::shared_ptr<BestOrder>, best_sell_order);
    QFETCH(std::shared_ptr<LastTransaction>, last_transaction);

    try
    {
        Responses::StockInfo stock_info(stock_id, best_buy_order, best_sell_order,
                                        last_transaction);

        Message::MessageLengthType should_be_message_length =
                sizeof(Message::MessageLengthType) +
                sizeof(Message::MessageType) +
                sizeof(StockIdType) +
                (best_buy_order ?
                    8 : 4) +
                (best_sell_order ?
                    8 : 4) +
                (last_transaction ?
                    last_transaction->lengthSerialized() : 4);

        QVERIFY2(stock_info.length() == should_be_message_length,
                 qPrintable(QString("Message length is incorrect. Should be %1 is %2.")
                            .arg(should_be_message_length)
                            .arg(stock_info.length())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}

void StockInfoTest::send_data()
{
    generate_valid_data();
}

void StockInfoTest::send()
{
    QFETCH(StockIdType, stock_id);
    QFETCH(std::shared_ptr<BestOrder>, best_buy_order);
    QFETCH(std::shared_ptr<BestOrder>, best_sell_order);
    QFETCH(std::shared_ptr<LastTransaction>, last_transaction);

    try
    {
        Responses::StockInfo stock_info(stock_id, best_buy_order, best_sell_order,
                                        last_transaction);

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        stock_info.send(stream.device());

        stream.device()->reset();


        Message::MessageLengthType should_be_bytes =
                sizeof(Message::MessageLengthType) +
                sizeof(Message::MessageType) +
                sizeof(StockIdType) +
                (best_buy_order ?
                    8 : 4) +
                (best_sell_order ?
                    8 : 4) +
                (last_transaction ?
                    last_transaction->lengthSerialized() : 4);

        QVERIFY2(stream.device()->size() == should_be_bytes,
                 qPrintable(QString("Bytes saved in device doesn't equal to "\
                                    "(supposedly) written. Should be %1 is %2.")
                            .arg(should_be_bytes)
                            .arg(stream.device()->size())));

        Message::MessageLengthType is_length;
        Message::MessageType       is_type;
        StockIdType                is_stock_id;

        stream >> is_length >> is_type >> is_stock_id;

        QVERIFY2(is_length == should_be_bytes,
                 qPrintable(QString("Message length doesn't match. Is %1 should be %2.")
                            .arg(is_length)
                            .arg(should_be_bytes)));
        assert (Message::RESPONSE_STOCK_INFO == 0x22);

        QVERIFY2(is_type == Message::RESPONSE_STOCK_INFO,
                 qPrintable(QString("Message type doesn't match. Is %1 should be %2.")
                            .arg(is_type)
                            .arg(Message::RESPONSE_STOCK_INFO)));

        QVERIFY2(is_stock_id == stock_id,
                         qPrintable(QString("Stock id doesn't match. Is %1 should be %2.")
                                    .arg(is_stock_id.value)
                                    .arg(stock_id.value)));
        if(!best_buy_order)
        {
            qint32 tmp;
            stream >> tmp;
            QVERIFY2(tmp == 0, "Guard value for best buy order is incorrect.");
        }
        else
        {
            AmountType is_best_buy_order_amount;
            PriceType is_best_buy_order_price;
            stream >> is_best_buy_order_amount >> is_best_buy_order_price;

            QVERIFY2(is_best_buy_order_amount     == best_buy_order->getAmount(),
                     qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                                .arg(is_best_buy_order_amount.value)
                                .arg(best_buy_order->getAmount().value)));
            QVERIFY2(is_best_buy_order_price     == best_buy_order->getPrice(),
                     qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                                .arg(is_best_buy_order_price.value)
                                .arg(best_buy_order->getPrice().value)));
        }
        if(!best_sell_order)
        {
            qint32 tmp;
            stream >> tmp;
            QVERIFY2(tmp == 0, "Guard value for best sell order is incorrect.");
        }
        else
        {
            AmountType is_best_sell_order_amount;
            PriceType is_best_sell_order_price;
            stream >> is_best_sell_order_amount >> is_best_sell_order_price;

            QVERIFY2(is_best_sell_order_amount    == best_sell_order->getAmount(),
                     qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                                .arg(is_best_sell_order_amount.value)
                                .arg(best_sell_order->getAmount().value)));
            QVERIFY2(is_best_sell_order_price    == best_sell_order->getPrice(),
                     qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                                .arg(is_best_sell_order_price.value)
                                .arg(best_sell_order->getPrice().value)));
        }
        if(!last_transaction)
        {
            qint32 tmp;
            stream >> tmp;
            QVERIFY2(tmp == 0, "Guard value for Last transaction is incorrect.");
        }
        else
        {
            LastTransaction is_last_transaction = LastTransaction::fromStream(stream);
            QVERIFY2(is_last_transaction.getAmount()     == last_transaction->getAmount(),
                     qPrintable(QString("Amount doesn't match. Is %1 should be %2.")
                                .arg(is_last_transaction.getAmount().value)
                                .arg(last_transaction->getAmount().value)));
            QVERIFY2(is_last_transaction.getPrice()      == last_transaction->getPrice(),
                     qPrintable(QString("Price doesn't match. Is %1 should be %2.")
                                .arg(is_last_transaction.getPrice().value)
                                .arg(last_transaction->getPrice().value)));
            QVERIFY2(is_last_transaction.getDateTime() == last_transaction->getDateTime(),
                     qPrintable(QString("Date time doesn't match. Is %1 should be %2.")
                                .arg(is_last_transaction.getDateTime())
                                .arg(last_transaction->getDateTime())));
        }
    }
    catch(std::invalid_argument e)
    {
        QFAIL(qPrintable(QString("Caught %1").arg(e.what())));
    }
    catch(...)
    {
        QFAIL("Exception has been thrown.");
    }
}
