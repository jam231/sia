#include <QCoreApplication>
#include <QString>


#include "market.h"

#include <configmanager.h>

int main(int argv, char **args)
{
    QCoreApplication app(argv, args);

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
