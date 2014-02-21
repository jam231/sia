#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QStringList>
#include <QDateTime>

#include <memory>

#include <writer.h>

#define LOG(call, message) call(message, __FILE__,__PRETTY_FUNCTION__,__LINE__)



enum LoggingLevel
{
    Warning,	// Error, Warning
    Info,		// all above + Info
    Debug,		// all above + Debug
    Trace,		// all above + Trace
    Off			// No logging at all, useful when testing since it's basically a mock (^^).
};



class AbstractLogger
{
public:
    virtual void error(QString message, QString file,
                       QString function, qint32 line) = 0;
    virtual void warning(QString message, QString file,
                         QString function, qint32 line) = 0;
    virtual void info(QString message, QString file,
                      QString function, qint32 line) = 0;
    virtual void debug(QString message, QString file,
                       QString function, qint32 line) = 0;
    virtual void trace(QString message, QString file,
                       QString function, qint32 line) = 0;
 protected:
        // Template method
    QString createLogEntry(QString loggingLevel, QString message,
                           QString file, QString function, qint32 line);

};

class DummyLogger : public AbstractLogger
{
    void error(QString, QString, QString, qint32) {}
    void warning(QString, QString, QString, qint32) {}
    void info(QString, QString, QString, qint32) {}
    void debug(QString, QString, QString, qint32) {}
    void trace(QString, QString, QString, qint32) {}
};


// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class WarningLogger : public DummyLogger
{
protected:
    std::unique_ptr<AbstractWriter> _writer;
public:
    WarningLogger(AbstractWriter* writer) : _writer(std::unique_ptr<AbstractWriter>(writer)) {}
    void error(QString message, QString file,
                QString function, qint32 line);
    void warning(QString message, QString file,
                QString function, qint32 line);
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class InfoLogger : public WarningLogger
{
public:
    InfoLogger(AbstractWriter* writer) : WarningLogger(writer) {}

    void info(QString message, QString file,
                QString function, qint32 line);
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class DebugLogger : public InfoLogger
{
public:
    DebugLogger(AbstractWriter* writer) : InfoLogger(writer) {}

    void debug(QString message, QString file,
                QString function, qint32 line);
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class TraceLogger : public DebugLogger
{
public:
    TraceLogger(AbstractWriter* writer) : DebugLogger(writer) {}

    void trace(QString message, QString file,
                QString function, qint32 line);
};


class LoggerFactory
{
public:
    LoggerFactory() {}
    AbstractLogger* create(LoggingLevel level, AbstractWriter* writer);
};



#endif // LOGGER_H
