#include "getmyordersmsg_test.h"

#include <Requests/getmyordersmsg.h>
#include <networkprotocol_utilities.h>

#include <utilities.h>

#include <stdexcept>
#include <memory>
#include <assert.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace Types;

void GetMyOrdersTest::initTestCase()
{
    GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(
                                    make_logger(LoggingLevel::Off))));
}

void GetMyOrdersTest::from_valid()
{

    try
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);

        assert(stream.byteOrder() == QDataStream::BigEndian);

        Message::MessageLengthType length =
                sizeof(Message::MessageLengthType) + sizeof(Message::MessageType);

        assert (length == 3);
        assert (Message::REQUEST_GET_MY_ORDERS == 0x1F);


        stream.device()->reset();

        Requests::GetMyOrders orders(stream);

        QVERIFY2(orders.length() == length,
                 qPrintable(QString("Length is incorrect. Is %1 should be %2.")
                            .arg(orders.length())
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
