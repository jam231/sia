#include <QCoreApplication>
#include <QString>
#include <QDateTime>



#include "DataStorage/postgresdatastorage.h"
#include "DataStorage/datahub.h"

#include "Servers/eventserver.h"
#include "master.h"

#include <configmanager.h>
#include <logger.h>


using namespace std;



int main(int argv, char **args)
{
    QCoreApplication app(argv, args);

    auto file_writer = shared_ptr<AbstractWriter>(new QDebugWriter());/*new FileWriter(QDate::currentDate()
                                                             .toString() + ".log"));*/
    auto composed_writer = new ThreadSafeWriter(move(file_writer));


    auto log_writer = shared_ptr<AbstractWriter>(composed_writer);

    auto local_logger = shared_ptr<AbstractLogger>(
                            make_logger(LoggingLevel::Trace, log_writer));

    auto logger_factory = shared_ptr<AbstractLoggerFactory>(
                        new SimpleLoggerFactory(LoggingLevel::Trace,
                                                  shared_ptr<AbstractWriter>(
                                                      log_writer)));


    LOG_INFO(local_logger, "Start.");

    try {

        QString config_file_path = "Files/config.ini";
        IniConfigManager config(config_file_path);

        auto settings = config.asQHash();

        auto postgre_data_factory = shared_ptr<AbstractDataStorageFactory>(
                    new PostgresDataStorageFactory(logger_factory, settings));

        auto db_connections_count_tmp = settings["database connections"];
        bool db_connections_can_convert;
        auto db_connections_count = db_connections_count_tmp.toInt(&db_connections_can_convert);

        if(!db_connections_can_convert || db_connections_count < 1)
        {
            throw std::runtime_error(QString("config for 'database connections' invalid value: %1.")
                                     .arg(db_connections_count_tmp)
                                     .toStdString());
        }

        auto master_data_factory = shared_ptr<AbstractDataStorageFactory>(
                   new PooledDataStorageFactory(logger_factory,
                                                 postgre_data_factory,
                                                 10));

        MasterServer master(logger_factory, master_data_factory, settings);
        master.start();
        master.setPriority(QThread::HighPriority);


        auto events_data_factory = shared_ptr<AbstractDataStorageFactory>(
                    new PooledDataStorageFactory(logger_factory, postgre_data_factory, 1));

        EventServer events(logger_factory, events_data_factory, settings);
        events.start();

        // Session handling must be handled without delays.
        events.setPriority(QThread::HighestPriority);

        PostgreDataHub notifications(logger_factory, settings);
        notifications.start();
        notifications.setPriority(QThread::HighPriority);

        master.connectDataHubToTradingServers(notifications);

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
