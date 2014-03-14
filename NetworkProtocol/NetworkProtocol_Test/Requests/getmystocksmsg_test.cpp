#include "getmystocksmsg_test.h"

#include <Requests/getmystocksmsg.h>
#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

void GetMyStocksTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void GetMyStocksTest::from_valid()
{

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType);

        assert (length == 3);
        assert (Message::REQUEST_GET_MY_STOCKS == 0x1D);


        stream.device()->reset();

        Requests::GetMyStocks stocks(stream);

        QVERIFY2(stocks.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(stocks.length())
                            .arg(length)));

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
