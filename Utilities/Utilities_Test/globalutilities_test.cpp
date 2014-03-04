#include "globalutilities_test.h"

#include <typeinfo>
#include <exception>
#include <stdexcept>
#include <memory>
#include <utility>

#include <utilities.h>
#include <logger.h>


#include <mockobjects.h>

using namespace std;

void GlobalUtilitiesTest::defaultGlobalLogger()
{
    AbstractLogger* global_logger = GlobalUtilities::getLogger().get();
    QVERIFY2(global_logger != nullptr, "Global logger is NULL.");

    DummyLogger* global_logger_as_DummyLogger = dynamic_cast<DummyLogger*>(global_logger);
    QVERIFY2(global_logger_as_DummyLogger != nullptr,
             "Default global logger should be of type DummyLogger.");
}

void GlobalUtilitiesTest::setLoggerWithNull()
{
    AbstractLogger* global_logger_before = GlobalUtilities::getLogger().get();
    try
    {
        GlobalUtilities::setLogger(nullptr);
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

    QTest::newRow("No logging")         << make_logger(LoggingLevel::Off,
                                                       move(shared_ptr<AbstractWriter>(new MockWriter())));
    QTest::newRow("Warning logging")    << make_logger(LoggingLevel::Warning,
                                                       move(shared_ptr<AbstractWriter>(new MockWriter())));
    QTest::newRow("Info logging")       << make_logger(LoggingLevel::Info,
                                                       move(shared_ptr<AbstractWriter>(new MockWriter())));
    QTest::newRow("Debug logging")      << make_logger(LoggingLevel::Debug,
                                                       move(shared_ptr<AbstractWriter>(new MockWriter())));
    QTest::newRow("Trace logging")      << make_logger(LoggingLevel::Trace,
                                                       move(shared_ptr<AbstractWriter>(new MockWriter())));
}

void GlobalUtilitiesTest::setLogger()
{
    AbstractLogger* global_logger_before = GlobalUtilities::getLogger().get();

    QFETCH(AbstractLogger*, logger);

    try
    {
        GlobalUtilities::setLogger(move(std::shared_ptr<AbstractLogger>(logger)));
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

