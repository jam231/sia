#ifndef LASTTRANSACTION_TEST_H
#define LASTTRANSACTION_TEST_H

#include <DataTransferObjects/lasttransaction.h>

#include <QtTest>

class LastTransactionTest : public QObject
{
    Q_OBJECT

public:
    LastTransactionTest();

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

#endif // LASTTRANSACTION_TEST_H
