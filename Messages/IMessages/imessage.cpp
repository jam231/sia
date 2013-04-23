#include "imessage.h"

#include <QDataStream>

IMessage::IMessage() : IOMessage()
{
}

IOMessage::MessageType IMessage::getMsgType(QIODevice* data)
{
    QDataStream tmpStream(data);
    qint32 tmpType;
    tmpStream>>tmpType;

    return toType(tmpType);
}

bool IMessage::isEnoughData(QIODevice* data)
{
    if(data->bytesAvailable() < 4)
        return false;

    QDataStream tmpStream(data->peek(4));
    qint32 length;
    tmpStream>>length;
    if(data->bytesAvailable() < length)
        return false;

    data->read(4);
    return true;
}
