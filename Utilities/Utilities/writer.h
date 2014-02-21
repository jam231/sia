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
    virtual void write(QString data) = 0;
    virtual ~AbstractWriter() {}
};

class FileWriter : public AbstractWriter
{
    QTextStream stream;
    std::unique_ptr<QFile> file;
public:

    FileWriter(QString file_name);
    // Not thread-safe
    void write(QString data);
    void flush();

    virtual ~FileWriter();
};

class StdInWriter : public AbstractWriter
{
    QTextStream* stream;
public:
    StdInWriter();
    // Not thread-safe
    void write(QString data);
    virtual ~StdInWriter();
};

class BufferedWriter : public AbstractWriter
{
    std::unique_ptr<AbstractWriter> _writer;
    QStringList _message_queue;
    qint32 _buffer_size;
public:
    // For better performance try tweaking the buffer_size value.
    BufferedWriter(AbstractWriter* writer, qint32 buffer_size=1024);

    // Not thread-safe
    void write(QString data);

    void flush();

    // Writes all remaining messages before destruction.
    virtual ~BufferedWriter();
};


class ConcurrentWriter : public AbstractWriter
{
    QMutex _lock;
    std::unique_ptr<AbstractWriter> _writer;
    QFuture<void> _previous_task;
public:

    // Writer's write must be thread-safe !!!
    ConcurrentWriter(AbstractWriter* writer);
    // Thread-safe
    void write(QString data);
};

#endif // WRITER_H
