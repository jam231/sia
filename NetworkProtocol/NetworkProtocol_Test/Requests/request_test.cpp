#include "request_test.h"

using namespace NetworkProtocol::Requests;

RequestMock::RequestMock()
{

}

RequestMock::RequestMock(QDataStream &in)) : Request(in)
{

}

