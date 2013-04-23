#ifndef UNRECOGNIZEDUSERMSG_H
#define UNRECOGNIZEDUSERMSG_H

#include "omessage.h"

#include <QIODevice>

class unrecognizedUserMsg : public OMessage
{

    qint32 length() const;
public:
    unrecognizedUserMsg();

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // UNRECOGNIZEDUSERMSG_H
