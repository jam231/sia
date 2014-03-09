#ifndef FAILUREMSG_TEST_H
#define FAILUREMSG_TEST_H


#include <QtTest>


/*
 *  Testing against specification in failuremsg.h
 */
class FailureTest : public QObject
{
    Q_OBJECT

    void generate_failure_types();

private Q_SLOTS:

    void initTestCase();

    void constant_length_data();
    void constant_length();

    void send_data();
    void send();

};

#endif // FAILUREMSG_TEST_H
