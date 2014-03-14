#ifndef BUYSTOCKMSG_TEST_H
#define BUYSTOCKMSG_TEST_H


#include <QtTest>


/*
 *  Testing against specification in buystockmsg.h
 */
class BuyStockTest : public QObject
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

#endif // BUYSTOCKMSG_TEST_H
