#include "order_test.h"

OrderTest::OrderTest()
{
}

void OrderTest::initTestCase()
{
}

void OrderTest::cleanupTestCase()
{
}

void OrderTest::testCase1()
{
    //QFETCH(QString, data);
    QVERIFY2(true, "This test should success.");
}

void OrderTest::testCase1_data()
{
    //QTest::addColumn<QString>("data");
    //QTest::newRow("0") << QString();
}
