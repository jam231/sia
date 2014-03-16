#ifndef MOCKOBJECTS_H
#define MOCKOBJECTS_H

#include <QString>
#include <QStringList>

#include <writer.h>

#include <assert.h>

class MockWriter : public AbstractWriter
{
public:
    std::shared_ptr<QStringList> buffer;
    MockWriter();
    MockWriter(const std::shared_ptr<QStringList>& buffer);
    void write(const QString &);
};

#endif // MOCKOBJECTS_H
