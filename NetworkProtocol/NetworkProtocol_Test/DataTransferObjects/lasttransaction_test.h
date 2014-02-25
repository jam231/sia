#ifndef LASTTRANSACTION_TEST_H
#define LASTTRANSACTION_TEST_H

#include <DataTransferObjects/lasttransaction.h>

#include <QtTest>

class LastTransactionTest : public QObject
{
    Q_OBJECT

public:
    LastTransactionTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // LASTTRANSACTION_TEST_H
