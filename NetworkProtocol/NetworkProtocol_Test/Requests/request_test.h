#ifndef REQUEST_TEST_H
#define REQUEST_TEST_H

#include "../NetworkProtocol/Requests/request.h"

class RequestMock : public NetworkProtocol::Requests::Request
{
public:
    RequestMock()
    {}
    RequestMock(QDataStream &in)
    {}
};

class RequestTest : public QObject
{
    Q_OBJECT

public:
    RequestTest();

private Q_SLOTS:
    void init_getTypeTest();
    void cleanup_getTypeTest();
    void getTypeTest();
    void getTypeTest_data();

    void init_getMessageLengthTest();
    void cleanup_getMessageLengthTest();
    void getMessageLengthTest();
    void getMessageLengthTest_data();

    void getMessageName();

    void init_validateRequestTest();
    void cleanup_validateRequestTest();
    void validateRequestTest();
    void validateRequestTest_data();
};

#endif // REQUEST_TEST_H
