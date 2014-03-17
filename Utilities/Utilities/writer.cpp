#include "writer.h"

#include <QDateTime>
#include <QIODevice>
#include <QtConcurrent/QtConcurrent>
#include <QString>
#include <QVariant>

#include <utility>
#include <stdexcept>
#include <assert.h>

template<class T>
AbstractWriter* operator<<(AbstractWriter* writer, const T& v)
{
    writer->write(QVariant(v).toString());
    return writer;
}

template<class T>
AbstractWriter& operator<<(AbstractWriter& writer, const T& v)
{
    writer.write(QVariant(v).toString());
    return writer;
}


StdInWriter::StdInWriter()
{
    stream = new QTextStream(stdout);
}

void StdInWriter::write(const QString &data)
{
    (*stream) << data;
}

StdInWriter::~StdInWriter()
{
   delete stream;
}

QDebugWriter::QDebugWriter()
{
}

void QDebugWriter::write(const QString &data)
{
    qDebug() << data;
}

QDebugWriter::~QDebugWriter()
{

}

FileWriter::FileWriter(QString file_name)
{
    file = std::unique_ptr<QFile>(new QFile(file_name));
    // We really don't want to lose previous logs due to bugs, so Append mode is neccessary.
    if ( !file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) )
    {
        throw std::runtime_error("FileWriter: " + std::string(file->errorString().toUtf8().constData()));
    }
    stream.setDevice(file.get());
}

void FileWriter::write(const QString &data)
{
    stream << data;
}

void FileWriter::flush()
{
    stream.flush();
}

FileWriter::~FileWriter()
{
    flush();
    file->close();
}


using namespace std;

UnboundedBufferWriter::UnboundedBufferWriter(shared_ptr<AbstractWriter> writer)
        : _writer(move(writer))
{
    if(!_writer)
    {
        throw std::invalid_argument("UnboundedBufferWriter (writer): writer can't be null.");
    }
}

void UnboundedBufferWriter::write(const QString &data)
{
    _message_queue << data;
}

void UnboundedBufferWriter::flush()
{
    if(!_message_queue.empty())
    {
        _writer->write(_message_queue.join(""));
        _message_queue.clear();
    }
}

UnboundedBufferWriter::~UnboundedBufferWriter()
{
    flush();
}


BufferedWriter::BufferedWriter(shared_ptr<AbstractWriter> writer, qint32 buffer_size)
        : _writer(move(writer)), _buffer_size(buffer_size)
{
    if(!_writer)
    {
        throw std::invalid_argument("BufferWriter(writer): writer can't be null.");
    }
}


void BufferedWriter::write(const QString &data)
{
    if(_message_queue.size() >= _buffer_size)
    {
        _writer->write(_message_queue.join(""));
        _message_queue.clear();
        assert (_message_queue.empty());
    }
    _message_queue << data;
}

void BufferedWriter::flush()
{
    if(!_message_queue.empty())
    {
        _writer->write(_message_queue.join(""));
        _message_queue.clear();
    }
}

BufferedWriter::~BufferedWriter()
{
    flush();
}

ThreadSafeWriter::ThreadSafeWriter(shared_ptr<AbstractWriter> writer)
    : _writer(move(writer))
{
    if(!_writer)
    {
        throw std::invalid_argument("ThreadSafeWriter(writer): writer can't be null.");
    }
}

void ThreadSafeWriter::write(const QString& data)
{
    _lock.lock();
    _writer->write(data);
    _lock.unlock();
}

AsyncWriter::AsyncWriter(shared_ptr<AbstractWriter> writer)
    : _writer(move(writer))
{
    if(!_writer)
    {
        throw std::invalid_argument("AsyncWriter(writer): writer can't be null.");
    }
}

void AsyncWriter::write(const QString &data)
{
    /* It is unacceptable to allow writing new batch
     * of messages when previous write request isn't finished.
     */
    _previous_task.waitForFinished();
    _previous_task = QtConcurrent::run(_writer.get(), &AbstractWriter::write, data);

}
