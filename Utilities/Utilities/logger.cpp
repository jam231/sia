#include "logger.h"


QString AbstractLogger::createLogEntry(QString loggingLevel, QString message,
                                       QString file, QString function, qint32 line)
{
    QStringList elements;
    elements << "[Date " << QDateTime::currentDateTime().toString() << "][FILE "
             << file << "][FUNCTION " << function << "][LINE " << QString::number(line)
             << "][" << loggingLevel << "] " << message << "\n";
    return elements.join("");
}


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





void InfoLogger::info(QString message, QString file,
            QString function, qint32 line)
{
    _writer->write(createLogEntry("Info", message, file, function, line));
}




void DebugLogger::debug(QString message, QString file,
            QString function, qint32 line)
{
    _writer->write(createLogEntry("Debug", message, file, function, line));
}




void TraceLogger::trace(QString message, QString file,
            QString function, qint32 line)
{
    _writer->write(createLogEntry("Trace", message, file, function, line));
}


AbstractLogger* LoggerFactory::create(LoggingLevel level, AbstractWriter* writer)
{
    switch(level)
    {
        case Warning:
            return new WarningLogger(writer);
        case Info:
            return new InfoLogger(writer);
        case Debug:
            return new DebugLogger(writer);
        case Trace:
            return new TraceLogger(writer);
        case Off:
            return new DummyLogger();
        default:
            throw std::invalid_argument("FactoryLogger.Create(level, writer) with level = unexpected value.");
    }
}
