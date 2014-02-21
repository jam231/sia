#ifndef BUFFEREDWRITERTEST_H
#define BUFFEREDWRITERTEST_H

#include <QString>
#include <QtTest>

#include "mockobjects.h"

class BufferedWriterTest : public QObject
{
    MockWriter _mock_writer;
    Q_OBJECT
private Q_SLOTS:

    void cleanup();

    void write_data();
    void write();

    void flush_data();
    void flush();

    void flushOnDestruction_data();
    void flushOnDestruction();
};

#endif // BUFFEREDWRITERTEST_H
