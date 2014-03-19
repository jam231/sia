#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QStringList>
#include <QDateTime>

#include <memory>
#include <assert.h>

#include <writer.h>

#define LOG_INFO(logger, message)       LOG(logger->info, message)
#define LOG_DEBUG(logger, message)      LOG(logger->debug, message)
#define LOG_ERROR(logger, message)      LOG(logger->error, message)
#define LOG_WARNING(logger, message)    LOG(logger->warning, message)
#define LOG_TRACE(logger, message)      LOG(logger->trace, message)

#define LOG(call, message) call(message,__FILE__,__PRETTY_FUNCTION__,__LINE__)



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
    std::shared_ptr<AbstractWriter> _writer;
public:
    WarningLogger(std::shared_ptr<AbstractWriter> writer);
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
    InfoLogger(std::shared_ptr<AbstractWriter> writer);

    void info(QString message, QString file,
                QString function, qint32 line);

    virtual ~InfoLogger() {}
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class DebugLogger : public InfoLogger
{
public:
    DebugLogger(std::shared_ptr<AbstractWriter> writer);

    void debug(QString message, QString file,
                QString function, qint32 line);

    virtual ~DebugLogger() {}
};

// I want the code to be DRY, hence this seemingly peculiar inheritance hierarchy.
class TraceLogger : public DebugLogger
{
public:
    TraceLogger(std::shared_ptr<AbstractWriter> writer);

    void trace(QString message, QString file,
                QString function, qint32 line);

    virtual ~TraceLogger() {}
};

class AbstractLoggerFactory
{
public:
    virtual std::shared_ptr<AbstractLogger> createLoggingSession() = 0;
    virtual ~AbstractLoggerFactory() {}
};

class SimpleLoggerFactory : public AbstractLoggerFactory
{
    LoggingLevel _level;
    std::shared_ptr<AbstractWriter> _writer;
public:
    SimpleLoggerFactory(LoggingLevel level, std::shared_ptr<AbstractWriter> writer);
    std::shared_ptr<AbstractLogger> createLoggingSession();
};

/*
 * Usage:
 *
 *      void method_1(std::shared_ptr<AbstractLogger> logger, int a, int b)
 *      {
 *          int c = a + b;
 *          LOG_TRACE(logger, QString("a(%1) + b(%c) = returned(%3).")
 *                            .arg(a).arg(b).arg(c));
 *          return c;
 *      }
 *      void some_method()
 *      {
 *          auto logger = loggerFactory.createLoggingSession();
 *          LOG_INFO(logger, "First entry.");
 *
 *          int n = method_1(logger, 1,2);
 *          LOG_DEBUG(logger, QString("method_1 returned %1").arg(n));
 *
 *          // some_method ends here - logger (i.e. shared_ptr) lifecycle ends
 *          // so all logs are now written to the next writer.
 *      }
 *
 */
class TransactionalLoggerFactory : public AbstractLoggerFactory
{
    LoggingLevel _level;
    std::shared_ptr<AbstractWriter> _writer;
public:
    TransactionalLoggerFactory(LoggingLevel level,
                               std::shared_ptr<AbstractWriter> writer);

    std::shared_ptr<AbstractLogger> createLoggingSession();
};



AbstractLogger *make_logger(LoggingLevel level,
                            std::shared_ptr<AbstractWriter> writer);

AbstractLogger *make_logger(LoggingLevel level);

#endif // LOGGER_H
