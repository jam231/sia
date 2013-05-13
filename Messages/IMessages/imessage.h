#ifndef IMESSAGE_H
#define IMESSAGE_H

#include "iomessage.h"

#include <QIODevice>

class IMessage : public IOMessage
{
public:
    static MessageType getMsgType(QIODevice* data);
    //static bool isEnoughData(QIODevice* data);

protected:
    //Nie potrzeba obiektow tego typu
    IMessage();
};

#endif // IMESSAGE_H
