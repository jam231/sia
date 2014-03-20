#include <QCoreApplication>
#include <QString>


//#include "market.h"

#include "master.h"

#include <QDateTime>

#include <configmanager.h>
#include <logger.h>

using namespace std;



int main(int argv, char **args)
{
    QCoreApplication app(argv, args);

    auto log_writer = shared_ptr<AbstractWriter>(new ThreadSafeWriter(
                                                     shared_ptr<AbstractWriter>(new QDebugWriter()
                                            /*(new FileWriter(QDate::currentDate()
                                                   .toString() + ".log")*/)));

    auto local_logger = shared_ptr<AbstractLogger>(
                            make_logger(LoggingLevel::Trace, log_writer));

    auto logger_factory = shared_ptr<AbstractLoggerFactory>(
                        new TransactionalLoggerFactory(LoggingLevel::Trace,
                                                  shared_ptr<AbstractWriter>(
                                                      move(log_writer))));


    LOG_INFO(local_logger, "Start.");
    // QThread::idealThreadCount()
    try {

        QString config_file_path = "Files/config.ini";
        IniConfigManager config(config_file_path);

        auto settings = config.asQHash();

        auto postgre_data_factory = shared_ptr<AbstractDataStorageFactory>(
                    new PostgresDataStorageFactory(logger_factory, settings));
/*
        auto data_factory = shared_ptr<AbstractDataStorageFactory>(
                    new PooledDataStorageFactory(logger_factory,
                                                 move(postgre_data_factory),
                                                 10));
*/
        MasterServer* master = new MasterServer(logger_factory,
                                                postgre_data_factory,
                                                settings);
        master->setAutoDelete(false);

        QThreadPool::globalInstance()->start(master, QThread::NormalPriority);
        /*
         * EventServer events(factory, settings);
         * events.setAutoDelete(false);
         * // It is ciritical to handle session open/close on without delays.
         * QThreadPool::globalInstance()->start(&events, QThread::HighestPriority);
         *
         *
         */
        return app.exec();


    }catch(const exception& e)
    {
        LOG_ERROR(local_logger, QString("Caught: %1").arg(e.what()));
        return 1;
    }
    catch(...)
    {
        LOG_ERROR(local_logger, "Unknown exception has been caught.");
        return 2;
    }
    return 0;
}
