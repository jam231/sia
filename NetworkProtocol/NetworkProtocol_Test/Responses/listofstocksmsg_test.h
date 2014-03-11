#ifndef LISTOFSTOCKSMSG_TEST_H
#define LISTOFSTOCKSMSG_TEST_H


#include <QtTest>


/*
 *  Testing against specification in listofstocksmsg.h
 */
class ListOfStocksTest : public QObject
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
#endif // LISTOFSTOCKSMSG_TEST_H
