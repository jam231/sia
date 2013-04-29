#include <QApplication>

#include "market.h"
#include "configmanager.h"

#include <QString>

int main(int argv, char **args)
{


    QApplication app(argv, args);

    // A bit ugly.
    QString configFilePath = "Files/serverConfig.ini";
    ConfigManager<> marketConfig(configFilePath);

    Market market(marketConfig);

    return app.exec();
}
