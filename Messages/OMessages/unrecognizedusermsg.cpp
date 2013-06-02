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
    sendHeader(out);
}
