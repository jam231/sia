#include "mockobjects.h"

MockWriter::MockWriter()
{
    buffer = std::shared_ptr<QStringList>(new QStringList());
}

MockWriter::MockWriter(const std::shared_ptr<QStringList>& buffer) : buffer(buffer)
{
    assert(buffer->empty());
}


void MockWriter::write(QString data)
{
    *buffer << data;
}
