#ifndef MOCKOBJECTS_H
#define MOCKOBJECTS_H

#include <QString>
#include <QStringList>

#include <writer.h>

#include <assert.h>

class MockWriter : public AbstractWriter
{
public:
    QStringList buffer;

    MockWriter();

    void write(QString);
};

#endif // MOCKOBJECTS_H
