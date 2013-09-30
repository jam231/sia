#include "loginuserrespfail.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

LoginUserRespFail::LoginUserRespFail(QString reason)
{
    m_reason = reason;
}

Types::MessageType LoginUserRespFail::type() const
{
    return Types::MessageType::LOGIN_USER_RESP_FAIL;
}

void LoginUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    QByteArray reason_bytes = m_reason.toUtf8();

    sendHeader(out);
    out << static_cast<Types::MessageLengthType>(reason_bytes.size());
    connection->write(reason_bytes);
}

Types::MessageLengthType LoginUserRespFail::length() const
{
    return sizeof(Types::MessageType) +
           m_reason.toUtf8().size()  + sizeof(Types::MessageLengthType);
}

const QString LoginUserRespFail::getMessageName()
{
    return Response::getMessageName() + " :: LoginUserRespFail";
}

}
}
