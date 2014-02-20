#ifndef BUYSTOCKMSG_TEST_H
#define BUYSTOCKMSG_TEST_H

#include "../NetworkProtocol/Requests/buystockmsg.h"

class BuyStockTest : public QObject
{
    Q_OBJECT

public:
    BuyStockTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // BUYSTOCKMSG_TEST_H
