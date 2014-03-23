#include "logger.h"

#include <stdexcept>
#include <utility>


using namespace std;


QString AbstractLogger::createLogEntry(QString loggingLevel, QString message,
                                       QString file, QString function, qint32 line)
{
    QStringList elements;
    elements << "[Date " << QDateTime::currentDateTime().toString() << "][FILE "
             << file << "][FUNCTION " << function << "][LINE " << QString::number(line)
             << "][" << loggingLevel << "] " << message << "\n";
    return elements.join("");
}




WarningLogger::WarningLogger(shared_ptr<AbstractWriter> writer)
    : _writer(move(writer))
{}

void  WarningLogger::error(QString message, QString file,
                           QString function, qint32 line)
{
    _writer->write(createLogEntry("Error", message, file, function, line));
}

void  WarningLogger::warning(QString message, QString file,
                             QString function, qint32 line)
{
    _writer->write(createLogEntry("Warning", message, file, function, line));
}




InfoLogger::InfoLogger(std::shared_ptr<AbstractWriter> writer)
    : WarningLogger(move(writer))
{}

void InfoLogger::info(QString message, QString file,
                      QString function, qint32 line)
{
    _writer->write(createLogEntry("Info", message, file, function, line));
}



DebugLogger::DebugLogger(shared_ptr<AbstractWriter> writer)
    : InfoLogger(move(writer))
{}

void DebugLogger::debug(QString message, QString file,
                        QString function, qint32 line)
{
    _writer->write(createLogEntry("Debug", message, file, function, line));
}



TraceLogger::TraceLogger(shared_ptr<AbstractWriter> writer)
    : DebugLogger(move(writer))
{}

void TraceLogger::trace(QString message, QString file,
                        QString function, qint32 line)
{
    _writer->write(createLogEntry("Trace", message, file, function, line));
}

AbstractLogger* make_logger(LoggingLevel level)
{
    return make_logger(level, std::shared_ptr<AbstractWriter>(new DummyWriter()));
}

AbstractLogger* make_logger(LoggingLevel level,
                            shared_ptr<AbstractWriter> writer)
{
    switch(level)
    {
    case Warning:
        return new WarningLogger(move(writer));
    case Info:
        return new InfoLogger(move(writer));
    case Debug:
        return new DebugLogger(move(writer));
    case Trace:
        return new TraceLogger(move(writer));
    case Off:
        return new DummyLogger();
    default:
        throw std::invalid_argument("make_logger(level, writer) with level = unexpected value.");
    }
}

SimpleLoggerFactory::SimpleLoggerFactory(LoggingLevel level,
                                         std::shared_ptr<AbstractWriter> writer)
    : _level(level), _writer(writer)
{
}

shared_ptr<AbstractLogger> SimpleLoggerFactory::createLoggingSession()
{
    return shared_ptr<AbstractLogger>(make_logger(_level, (_writer)));
}


TransactionalLoggerFactory::TransactionalLoggerFactory(LoggingLevel level,
                                                       std::shared_ptr<AbstractWriter> writer)
    : _level(level), _writer(std::move(writer))
{
    if(!_writer)
    {
        throw std::invalid_argument("TransactionalLoggerFactory(writer): writer can't be null.");
    }
}

std::shared_ptr<AbstractLogger> TransactionalLoggerFactory::createLoggingSession()
{
    auto writer = std::shared_ptr<AbstractWriter>(
                                            new UnboundedBufferWriter(_writer));
    auto logger = std::shared_ptr<AbstractLogger>(make_logger(_level, writer));

    return logger;
}
