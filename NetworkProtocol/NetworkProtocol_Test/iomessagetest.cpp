#include "iomessagetest.h"

IOMessageTest::IOMessageTest()
{
}

void IOMessageTest::initTestCase()
{
}

void IOMessageTest::cleanupTestCase()
{
}

void IOMessageTest::testCase1()
{
    //QFETCH(QString, data);
    QVERIFY2(true, "This test should success.");
}

void IOMessageTest::testCase1_data()
{
    //QTest::addColumn<QString>("data");
    //QTest::newRow("0") << QString();
}
