#ifndef LISTOFORDERSMSG_TEST_H
#define LISTOFORDERSMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in listofordersmsg.h
 */
class ListOfOrdersTest : public QObject
{
    Q_OBJECT

    void generate_valid_data();

private Q_SLOTS:

    void initTestCase();

    void length_data();
    void length();

    void send_data();
    void send();
};

#endif // LISTOFORDERSMSG_TEST_H
