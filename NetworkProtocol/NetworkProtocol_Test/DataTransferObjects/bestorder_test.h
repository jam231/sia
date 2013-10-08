#ifndef BESTORDER_TEST_H
#define BESTORDER_TEST_H

#include "../NetworkProtocol/DataTransferObjects/bestorder.h"

#include <QtTest>

class BestOrderTest : public QObject
{
    Q_OBJECT

public:
    BestOrderTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // BESTORDER_TEST_H
