#ifndef BESTORDER_TEST_H
#define BESTORDER_TEST_H

#include "DataTransferObjects/bestorder.h"

#include <QtTest>

class BestOrderTest : public QObject
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

    void toStrea_valid_data();
    void toStrea_valid();

    void fromStrea_valid_data();
    void fromStrea_valid();

    void fromStrea_invalid_data();
    void fromStrea_invalid();

    void lengthSerialized_data();
    void lengthSerialized();
};

#endif // BESTORDER_TEST_H
