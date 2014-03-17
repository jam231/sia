#ifndef WRITER_H
#define WRITER_H


#include <QObject>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>
#include <QTextStream>

#include <memory>

class AbstractWriter
{
public:
    virtual void write(const QString&) = 0;
    virtual ~AbstractWriter() {}
};

template<class T>
AbstractWriter* operator<<(AbstractWriter*, const T&);

template<class T>
AbstractWriter& operator<<(AbstractWriter&, const T&);


class DummyWriter : public AbstractWriter
{
public:
    void write(const QString&) {}
};

class FileWriter : public AbstractWriter
{
    QTextStream stream;
    std::unique_ptr<QFile> file;
public:

    FileWriter(QString file_name);
    // Not thread-safe
    void write(const QString& data);
    void flush();

    virtual ~FileWriter();
};

class StdInWriter : public AbstractWriter
{
    QTextStream* stream;
public:
    StdInWriter();
    // Not thread-safe
    void write(const QString &data);
    virtual ~StdInWriter();
};

class QDebugWriter : public AbstractWriter
{
public:
    QDebugWriter();
    // Not thread-safe
    void write(const QString &data);
    virtual ~QDebugWriter();
};

/*
 *
 *  When buffer_size <= 2 then factual buffer size will be 1
 *
 *  Write to underlying buffer is requested when
 *  after requesting new write buffer contains buffer_size elements.
 */
class BufferedWriter : public AbstractWriter
{
    std::shared_ptr<AbstractWriter> _writer;
    QStringList _message_queue;
    qint32 _buffer_size;
public:
    // For better performance try tweaking the buffer_size value.
    BufferedWriter(std::shared_ptr<AbstractWriter> writer, qint32 buffer_size=1024);

    // Not thread-safe
    void write(const QString &data);

    void flush();

    // Writes all remaining messages before destruction.
    virtual ~BufferedWriter();
};

class UnboundedBufferWriter : public AbstractWriter
{
    std::shared_ptr<AbstractWriter> _writer;
    QStringList _message_queue;
public:
    UnboundedBufferWriter(std::shared_ptr<AbstractWriter> writer);

    // Not thread-safe
    void write(const QString &data);

    void flush();

    // Writes all remaining messages before destruction.
    virtual ~UnboundedBufferWriter();
};

class ThreadSafeWriter : public AbstractWriter
{
       QMutex _lock;
       std::shared_ptr<AbstractWriter> _writer;
public:
       ThreadSafeWriter(std::shared_ptr<AbstractWriter> writer);
       // Thread-safe
       void write(const QString &data);
};

class AsyncWriter : public AbstractWriter
{

    std::shared_ptr<AbstractWriter> _writer;
    QFuture<void> _previous_task;
public:
    AsyncWriter(std::shared_ptr<AbstractWriter> writer);
    // Not thread-safe
       void write(const QString &data);
};

#endif // WRITER_H
