#ifndef GETMYSTOCKSMSG_TEST_H
#define GETMYSTOCKSMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in getmyordersmsg.h
 */
class GetMyStocksTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void from_valid();

};

#endif // GETMYSTOCKSMSG_TEST_H
