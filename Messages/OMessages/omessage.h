#ifndef OMESSAGE_H
#define OMESSAGE_H

#include "iomessage.h"

#include <QIODevice>

class OMessage : public IOMessage
{
protected:
    virtual qint32 length() const = 0;
    OMessage();

public:
    virtual void send(QIODevice* connection) = 0;
};

#endif // OMESSAGE_H
