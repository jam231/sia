#ifndef NETWORKPROTOCOLUTILITIES_TEST_H
#define NETWORKPROTOCOLUTILITIES_TEST_H


#include <QtTest>


/*
 *  Testing functions from networkprotocol_utilities.h
 */
class NetworkProtocolUtilitiesTest : public QObject
{
    Q_OBJECT

    void _readLength_valid_data();
    void _readLength_invalid_data();

    void _getLength_valid_data();
    void _getLength_invalid_data();

    void _readType_valid_data();
    void _readType_invalid_data();


private Q_SLOTS:

    void initTestCase();

    void getLength_valid_data();
    void getLength_invalid_data();

    void getLength_valid();
    void getLength_invalid();

    void readLength_valid_data();
    void readLength_invalid_data();

    void readLength_valid();
    void readLength_invalid();

    void readType_valid_data();
    void readType_invalid_data();

    void readType_valid();
    void readType_invalid_empty();
    void readType_invalid();

    void readType_NoEx_valid_data();
    void readType_NoEx_invalid_data();

    void readType_NoEx_valid();
    void readType_NoEx_invalid_empty();
    void readType_NoEx_invalid();

    void fromStream_valid_data();
    void fromStream_invalid_incomplete_data();


    void fromStream_valid();
    void fromStream_invalid_incomplete();

    void fromStream_invalid_body_data();
    void fromStream_invalid_body();

    void fromStream_malformed_data();
    void fromStream_malformed();
};


#endif // NETWORKPROTOCOLUTILITIES_TEST_H
