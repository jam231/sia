#ifndef LASTTRANSACTIONMSG_TEST_H
#define LASTTRANSACTIONMSG_TEST_H


#include <QtTest>


/*
 *  Testing against specification in lasttransactionmsg.h
 */
class LastTransactionResponseTest : public QObject
{
    Q_OBJECT

    void generate_valid_data();
    void generate_invalid_data();

private Q_SLOTS:

    void initTestCase();

    void creation_valid_data();
    void creation_valid();

    void creation_invalid_data();
    void creation_invalid();

    void length_data();
    void length();

    void send_data();
    void send();

};
#endif // LASTTRANSACTIONMSG_TEST_H
