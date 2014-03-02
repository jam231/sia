#ifndef ORDER_TEST_H
#define ORDER_TEST_H

#include "../NetworkProtocol/DataTransferObjects/order.h"

#include <QtTest>

class OrderTest : public QObject
{
    Q_OBJECT

public:
    OrderTest();

private Q_SLOTS:
    void initTestCase();

    void creation_valid_data();
    void creation_valid();

    void creation_invalid_data();
    void creation_invalid();

    void toStream_valid_data();
    void toStream_valid();

    void fromStream_valid_data();
    void fromStream_valid();

    void fromStream_invalid_data();
    void fromStream_invalid();

    void lengthSerialized_data();
    void lengthSerialized();
};

#endif // ORDER_TEST_H
