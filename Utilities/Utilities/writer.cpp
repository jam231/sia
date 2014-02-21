#include "writer.h"

#include <QDateTime>
#include <QIODevice>
#include <QtConcurrent/QtConcurrent>
#include <QString>


#include <stdexcept>
#include <assert.h>


/// TODO: Destructors seem not to be called on delete.
/// TODO: Investigate why StdInWriter don't work when using stream.
StdInWriter::StdInWriter()
{
    stream = new QTextStream(stdout);
}

void StdInWriter::write(QString data)
{
    (*stream) << data;
}

StdInWriter::~StdInWriter()
{
   delete stream;
}



FileWriter::FileWriter(QString file_name)
{
    file = std::unique_ptr<QFile>(new QFile(file_name));
    // We really don't want to lose previous logs due to bugs, so Append mode is required.
    if ( !file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) )
    {
        throw std::runtime_error("FileWriter: " + std::string(file->errorString().toUtf8().constData()));
    }
    stream.setDevice(file.get());
}

void FileWriter::write(QString data)
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







BufferedWriter::BufferedWriter(AbstractWriter* writer, qint32 buffer_size)
        : _writer(std::unique_ptr<AbstractWriter>(writer)), _buffer_size(buffer_size)
{
    if(!_writer)
    {
        throw std::invalid_argument("BufferWriter(writer): writer can't be null.");
    }
}


void BufferedWriter::write(QString data)
{
    _message_queue << data;
    if(_message_queue.size() >= _buffer_size)
    {
        _writer->write(_message_queue.join(""));
        _message_queue.clear();
        assert (_message_queue.empty());
    }
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




ConcurrentWriter::ConcurrentWriter(AbstractWriter* writer)
    : _writer(std::unique_ptr<AbstractWriter>(writer))
{
    if(!_writer)
    {
        throw std::invalid_argument("ConcurrentWriter(writer): writer can't be null.");
    }
}

void ConcurrentWriter::write(QString data)
{
    /* It is unacceptable to allow writing new batch
     * of messages when previous write request isn't finished.
     */
    _lock.lock();
        _previous_task.waitForFinished();
        _previous_task = QtConcurrent::run(_writer.get(), &AbstractWriter::write, data);
    _lock.unlock();
}
