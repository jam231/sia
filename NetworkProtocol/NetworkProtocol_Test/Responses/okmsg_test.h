#ifndef OKMSG_TEST_H
#define OKMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in okmsg.h
 */
class OkTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void constant_length();

    void send();

};

#endif // OKMSG_TEST_H
