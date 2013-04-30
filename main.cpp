#include <QApplication>
#include <QString>


#include "exceptionhierarchy.h"
#include "market.h"
#include "configmanager.h"


int main(int argv, char **args)
{


    //QApplication app(argv, args);

    try {

        QString configFilePath = "Files/config.ini";
        /*
         * I will buy a beer for the first person who
         * can explain why is it when I define
         *      ConfigManager<> config(...) "on-fly"
         * like that:
         *      Market market(ConfigManager<>(...))
         * It seems like the program is skipping to return 0,
         * doing nothing in-between, including not initializing object
         * market.
         */
        ConfigManager<> config(configFilePath);
        Market market(config);

    }catch(const std::exception& e)
    {
        qDebug() << "[main] Exception was thrown with message: " << e.what();
        return 1;
    }
    catch(...)
    {
        qDebug() << "[main] Unknown exception detected.";
    }
    return 0;
    //return app.exec();    //That's just wrong.
}
