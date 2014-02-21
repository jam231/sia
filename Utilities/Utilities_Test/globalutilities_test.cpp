#include "globalutilities_test.h"

#include <typeinfo>
#include <exception>
#include <stdexcept>

#include <utilities.h>
#include <logger.h>

#include <mockobjects.h>


void GlobalUtilitiesTest::defaultGlobalLogger()
{
    AbstractLogger* global_logger = GlobalUtilities::getLogger().get();
    QVERIFY2(global_logger != NULL, "Global logger is NULL.");

    DummyLogger* global_logger_as_DummyLogger = dynamic_cast<DummyLogger*>(global_logger);
    QVERIFY2(global_logger_as_DummyLogger != NULL, "Default global logger should be of type DummyLogger.");
}

void GlobalUtilitiesTest::setLoggerWithNull()
{
    AbstractLogger* global_logger_before = GlobalUtilities::getLogger().get();
    try
    {
        GlobalUtilities::setLogger(NULL);
        QFAIL("Exception hasn't been thrown.");
    }
    catch(const std::invalid_argument& e)
    {
        QVERIFY2(global_logger_before == GlobalUtilities::getLogger().get(),
                 "Global logger shouldn't change.");
    }
    catch(...)
    {
        QFAIL("Cought unkown exception.");
    }
}
Q_DECLARE_METATYPE(AbstractLogger*)

void GlobalUtilitiesTest::setLogger_data()
{

    QTest::addColumn<AbstractLogger*>("logger");

    LoggerFactory* factory = new LoggerFactory();

    QTest::newRow("No logging") << factory->create(LoggingLevel::Off, new MockWriter());
    QTest::newRow("Warning logging") << factory->create(LoggingLevel::Warning, new MockWriter());
    QTest::newRow("Info logging") << factory->create(LoggingLevel::Info, new MockWriter());
    QTest::newRow("Debug logging") << factory->create(LoggingLevel::Debug, new MockWriter());
    QTest::newRow("Trace logging") << factory->create(LoggingLevel::Trace, new MockWriter());

    delete factory;
}

void GlobalUtilitiesTest::setLogger()
{
    AbstractLogger* global_logger_before = GlobalUtilities::getLogger().get();

    QFETCH(AbstractLogger*, logger);

    try
    {
        GlobalUtilities::setLogger(logger);
        QVERIFY2(logger == GlobalUtilities::getLogger().get(),
                 qPrintable("Failed to set global logger. Global logger is set to " +
                    QString(typeid(GlobalUtilities::getLogger().get()).name())));
    }
    catch(const std::exception& e)
    {
        QFAIL(qPrintable("Exception " + QString(e.what()) + " has been thrown"));
        QVERIFY2(global_logger_before == GlobalUtilities::getLogger().get(),
                 "Global logger shouldn't change.");
    }
    catch(...)
    {
        QFAIL("Cought unkown exception.");
    }
}

