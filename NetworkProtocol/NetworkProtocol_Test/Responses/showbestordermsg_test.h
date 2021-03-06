#ifndef BESTORDERMSG_TEST_H
#define BESTORDERMSG_TEST_H


#include <QtTest>


/*
 *  Testing against specification in showbestordermsg.h
 */
class ShowBestOrderTest : public QObject
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
#endif // BESTORDERMSG_TEST_H
