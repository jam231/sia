#ifndef IMESSAGE_H
#define IMESSAGE_H

#include "iomessage.h"

#include <QIODevice>

#include <exception>

class InvalidDataInMsg: public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Invalid data in message.";
    }
};


class IMessage : public IOMessage
{
public:
    static MessageType getMsgType(QDataStream &in);
    //static bool isEnoughData(QIODevice* data);
    static qint16 getMsgLength(QIODevice* data);
protected:
    //Nie potrzeba obiektow tego typu
    IMessage();
public:
    virtual ~IMessage() {}
};

#endif // IMESSAGE_H
