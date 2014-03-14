#ifndef GETSTOCKINFOMSG_TEST_H
#define GETSTOCKINFOMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in getstockinfomsg.h
 */
class GetStockInfoTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void from_valid_data();
    void from_valid();

    void from_invalid_body_data();
    void from_invalid_body();


    void from_malformed_data();
    void from_malformed();
};

#endif // GETSTOCKINFOMSG_TEST_H
