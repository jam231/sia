#ifndef CANCELORDERMSG_TEST_H
#define CANCELORDERMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in cancelordermsg.h
 */
class CancelOrderTest : public QObject
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
#endif // CANCELORDERMSG_TEST_H
