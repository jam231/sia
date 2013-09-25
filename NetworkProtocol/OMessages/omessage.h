#ifndef OMESSAGE_H
#define OMESSAGE_H

#include "iomessage.h"

#include <QIODevice>
#include <QDataStream>

namespace NetworkProtocol {

class  OMessage : public IOMessage
{
protected:
    virtual qint16 length() const = 0;
    void sendHeader(QDataStream& out);

    OMessage();

public:
    virtual void send(QIODevice* connection) = 0;
    virtual ~OMessage() {}
};

}
#endif // OMESSAGE_H
