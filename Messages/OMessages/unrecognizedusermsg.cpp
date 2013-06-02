#include "unrecognizedusermsg.h"

#include <QDataStream>

UnrecognizedUserMsg::UnrecognizedUserMsg()
{}

IOMessage::MessageType UnrecognizedUserMsg::type() const
{
    return UNRECOGNIZED_USER;
}

qint16 UnrecognizedUserMsg::length() const
{

    return sizeof(MessageType);
}


void UnrecognizedUserMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);
    // Nie wiem czemu trzeba tu robić cast'a
    // IOMessage:MessageType to enum, który ma typ qint8
    // jednak bez cast'a strumien traktuje type() jako 4 bajty.
    out << static_cast<qint8>(type());
}
