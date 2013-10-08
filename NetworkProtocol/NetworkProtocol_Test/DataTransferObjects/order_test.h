#ifndef ORDER_TEST_H
#define ORDER_TEST_H

#include "../NetworkProtocol/DataTransferObjects/order.h"

#include <QtTest>

class OrderTest : public QObject
{
    Q_OBJECT

public:
    OrderTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // ORDER_TEST_H
