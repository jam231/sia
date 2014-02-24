#ifndef BUFFEREDWRITERTEST_H
#define BUFFEREDWRITERTEST_H

#include <QString>
#include <QtTest>

#include "mockobjects.h"

#include <memory>

class BufferedWriterTest : public QObject
{
    std::unique_ptr<MockWriter> _mock_writer;
    Q_OBJECT
private Q_SLOTS:

    void init();

    void write_data();
    void write();

    void flush_data();
    void flush();

    void flushOnDestruction_data();
    void flushOnDestruction();
};

#endif // BUFFEREDWRITERTE_H
