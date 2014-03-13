#ifndef SHOWORDERMSG_TEST_H
#define SHOWORDERMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in newordermsg.h
 */
class ShowNewOrderTest : public QObject
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

    void constant_length_data();
    void constant_length();

    void send_data();
    void send();

};

#endif // SHOWORDERMSG_TEST_H
