#include <QCoreApplication>
#include <QString>


#include "market.h"


#include <QDateTime>

#include <configmanager.h>
#include <utilities.h>
#include <logger.h>

int main(int argv, char **args)
{
    QCoreApplication app(argv, args);

    // Malutki test celem sprawdzanie czy wszystko sie dolacza, bo byly z tym problemy.
    AbstractLoggerFactory* factory = new SimpleLoggerFactory(LoggingLevel::Trace,
                                                             std::shared_ptr<AbstractWriter>(
                                                                 new FileWriter(
                                                                     QDate::currentDate().toString() + ".log")
                                                                 ));
    GlobalUtilities::setLogger(factory->createLoggingSession());
    delete factory;

    GLOBAL_LOG_INFO("-------------------------------START-------------------------------");

    try {

        QString configFilePath = "Files/config.ini";
        ConfigManager<> config(configFilePath);


        //Market market(config);

        return app.exec();


    }catch(const std::exception& e)
    {
        qDebug() << "[main] Złapano wyjątek: "
                 << e.what();

        return 1;
    }
    catch(...)
    {
        qDebug() << "[main] Załapano nieznany wyjątek.";

        return 2;
    }
    return 0;
}
