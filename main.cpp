#include <QApplication>

#include "market.h"



int main(int argv, char **args)
{

    QApplication app(argv, args);

    Market market;

    return app.exec();
}
