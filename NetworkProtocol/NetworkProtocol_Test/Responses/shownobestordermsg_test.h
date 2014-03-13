#ifndef SHOWNOBESTORDERMSG_TEST_H
#define SHOWNOBESTORDERMSG_TEST_H


#include <QtTest>


/*
 *  Testing against specification in orderrealizationmsg.h
 */
class ShowNoBestOrderTest : public QObject
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

#endif // SHOWNOBESTORDERMSG_TEST_H
