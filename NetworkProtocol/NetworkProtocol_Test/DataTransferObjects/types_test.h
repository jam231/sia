#ifndef TYPES_TEST_H
#define TYPES_TEST_H

#include "../NetworkProtocol/DataTransferObjects/types.h"

#include <QtTest>

class TypesNamespaceFunctionsTest : public QObject
{
    Q_OBJECT

public:
    TypesNamespaceFunctionsTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

class OrderIdTypeTest : public QObject
{
    Q_OBJECT

public:
    OrderIdTypeTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

class StockIdTypeTest : public QObject
{
    Q_OBJECT

public:
    StockIdTypeTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

class AmountTypeTest : public QObject
{
    Q_OBJECT

public:
    AmountTypeTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

class CompanyIdTypeTest : public QObject
{
    Q_OBJECT

public:
    CompanyIdTypeTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

class UserIdTypeTest : public QObject
{
    Q_OBJECT

public:
    UserIdTypeTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

class PriceTypeTest : public QObject
{
    Q_OBJECT

public:
    PriceTypeTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};



#endif // TYPES_TEST_H
