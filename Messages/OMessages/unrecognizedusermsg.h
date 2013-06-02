#ifndef UNRECOGNIZEDUSERMSG_H
#define UNRECOGNIZEDUSERMSG_H

#include "omessage.h"

#include <QIODevice>

class UnrecognizedUserMsg : public OMessage
{

    qint32 length() const;
public:
    UnrecognizedUserMsg();

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // UNRECOGNIZEDUSERMSG_H
