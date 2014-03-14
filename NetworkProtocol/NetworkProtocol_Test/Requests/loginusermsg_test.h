#ifndef LOGINUSERMSG_TEST_H
#define LOGINUSERMSG_TEST_H

#include <QtTest>


/*
 *  Testing against specification in loginusermsg.h
 */
class LoginUserTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void from_valid_data();
    void from_valid();

    void from_invalid_body_data();
    void from_invalid_body();


    void from_malformed_data();
    void from_malformed();
};

#endif // LOGINUSERMSG_TEST_H
