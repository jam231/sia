#ifndef ORDERCOMPLETEDMSG_TEST_H
#define ORDERCOMPLETEDMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in ordercompletedmsg.h
 */
class OrderCompletedTest : public QObject
{
    Q_OBJECT

    void generate_valid_data();

private Q_SLOTS:

    void initTestCase();

    void constant_length_data();
    void constant_length();

    void send_data();
    void send();


    void creation_valid_data();
    void creation_valid();

    void creation_invalid_data();
    void creation_invalid();
};

#endif // ORDERCOMPLETEDMSG_TEST_H
