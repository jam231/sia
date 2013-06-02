#include "omessage.h"

OMessage::OMessage() : IOMessage()
{
}

void OMessage::sendHeader(QDataStream& out)
{
    out << static_cast<qint16>(length()) << static_cast<qint8>(type());
}
