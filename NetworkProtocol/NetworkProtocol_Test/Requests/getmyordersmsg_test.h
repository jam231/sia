#ifndef GETMYORDERSMSG_TEST_H
#define GETMYORDERSMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in getmyordersmsg.h
 */
class GetMyOrdersTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void from_valid();

};

#endif // GETMYORDERSMSG_TEST_H
