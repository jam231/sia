#include "mockobjects.h"

MockWriter::MockWriter()
{
    assert(buffer.empty());
}

void MockWriter::write(QString data)
{
    buffer << data;
}
