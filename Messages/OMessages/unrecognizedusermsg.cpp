#include "unrecognizedusermsg.h"

#include <QDataStream>

unrecognizedUserMsg::unrecognizedUserMsg()
{}

IOMessage::MessageType unrecognizedUserMsg::type() const
{
    return UNRECOGNIZED_USER;
}

qint32 unrecognizedUserMsg::length() const
{

    return sizeof(MessageType);
}


void unrecognizedUserMsg::send(QIODevice* connection)
{
    QDataStream out(connection);
    out.setByteOrder(QDataStream::BigEndian);
    out << type();
}
