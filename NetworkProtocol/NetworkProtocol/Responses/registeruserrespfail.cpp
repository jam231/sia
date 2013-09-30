#include "registeruserrespfail.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

RegisterUserRespFail::RegisterUserRespFail(QString reason)
    : m_reason(reason)
{
    m_reason = reason;
}

Types::MessageLengthType RegisterUserRespFail::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(Types::MessageLengthType) + m_reason.toUtf8().size();
}

Types::MessageType RegisterUserRespFail::type() const
{
    return Types::MessageType::RESPONSE_REGISTER_USER_FAIL;
}

void RegisterUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    QByteArray reason_bytes = m_reason.toUtf8();

    sendHeader(out);

    out << static_cast<Types::MessageLengthType>(reason_bytes.size());
    connection->write(reason_bytes);
}

}
}
