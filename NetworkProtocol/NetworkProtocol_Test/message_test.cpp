#include "message_test.h"

MessageTest::MessageTest()
{
}

void MessageTest::initTestCase()
{
}

void MessageTest::cleanupTestCase()
{
}

void MessageTest::testCase1()
{
    //QFETCH(QString, data);
    QVERIFY2(true, "This test should success.");
}

void MessageTest::testCase1_data()
{
    //QTest::addColumn<QString>("data");
    //QTest::newRow("0") << QString();
}
