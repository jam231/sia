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
    virtual ~AbstractLogger() {}
 protected:
        // Template method
    QString createLogEntry(QString loggingLevel, QString message,
                           QString file, QString function, qint32 line);

};


class DummyLogger : public AbstractLogger
{
public:
    DummyLogger() {}
    void error(QString, QString, QString, qint32) {}
    void warning(QString, QString, QString, qint32) {}
    void info(QString, QString, QString, qint32) {}
    void debug(QString, QString, QString, qint32) {}
    void trace(QString, QString, QString, qint32) {}

    virtual ~DummyLogger() {}
};


// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class WarningLogger : public DummyLogger
{
protected:
    std::unique_ptr<AbstractWriter> _writer;
public:
    WarningLogger(std::unique_ptr<AbstractWriter> writer);
    void error(QString message, QString file,
                QString function, qint32 line);
    void warning(QString message, QString file,
                QString function, qint32 line);

    virtual ~WarningLogger() {}
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class InfoLogger : public WarningLogger
{
public:
    InfoLogger(std::unique_ptr<AbstractWriter> writer);

    void info(QString message, QString file,
                QString function, qint32 line);

    virtual ~InfoLogger() {}
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class DebugLogger : public InfoLogger
{
public:
    DebugLogger(std::unique_ptr<AbstractWriter> writer);

    void debug(QString message, QString file,
                QString function, qint32 line);

    virtual ~DebugLogger() {}
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class TraceLogger : public DebugLogger
{
public:
    TraceLogger(std::unique_ptr<AbstractWriter> writer);

    void trace(QString message, QString file,
                QString function, qint32 line);

    virtual ~TraceLogger() {}
};


class LoggerFactory
{
public:
    AbstractLogger* create(LoggingLevel level,
                           std::unique_ptr<AbstractWriter> writer = std::unique_ptr<AbstractWriter>(new DummyWriter()));
};

AbstractLogger* make_logger(LoggingLevel level,
                            std::unique_ptr<AbstractWriter> writer = std::unique_ptr<AbstractWriter>(new DummyWriter()));



#endif // LOGGER_H
