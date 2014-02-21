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
    LoggerFactory* factory = new LoggerFactory();
    GlobalUtilities::setLogger(factory->create(LoggingLevel::Trace,
                                               new FileWriter(
                                                   QDate::currentDate().toString() + ".log"
                                               ))
                               );
    delete factory;

    LOG_INFO("-------------------------------START-------------------------------");

    try {

        QString configFilePath = "Files/config.ini";
        ConfigManager<> config(configFilePath);


        Market market(config);

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
