#ifndef BESTORDER_TEST_H
#define BESTORDER_TEST_H

#include "DataTransferObjects/bestorder.h"

#include <QtTest>

class BestOrderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void creation_valid_data();
    void creation_valid();

    void creation_invalid_data();
    void creation_invalid();

    void operatorLSH_valid_data();
    void operatorLSH_valid();

    void operatorLSH_invalid_data();
    void operatorLSH_invalid();

    void operatorRSH_valid_data();
    void operatorRSH_valid();

    void operatorRSH_invalid_data();
    void operatorRSH_invalid();

    void lengthInBytes_data();
    void lengthInBytes();
};

#endif // BESTORDER_TEST_H
