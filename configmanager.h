#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "Utility.h"

#include <QString>
#include <QHash>
#include <QDebug>

/*
 *  ConfigManager reads configuration file from a given path.
 *  When it has a set of Key, Value pairs it can be queried
 *  in given manner:
 *
 *      int port = myConfig["SERVER PORT"];
 *      bool hasServerIp = myConfig.contains("SERVER IP");
 *
 *  It is ConfigManger's role to check whether VALUE held by a given
 *  key can be translated to an output type.
 *
 */


/*
 *  Fun fact:   You cannot partialy specialize template methods
 *              if without specializing class.
 *  Func fact2: You cannot partialy specialize template methods !
 *
 */

template<typename Container = QHash<QString, QString> >
class ConfigManager
{
    void parseAndBuildFromLines(QStringList lines);

    Container m_Container;
public:
    ConfigManager(const QString& pathToConfigFile);
    Maybe<QString> operator[](const QString& key) const;
    Maybe<int> intAt(const QString&) const;

    bool contains(const QString&) const;
};
#endif // CONFIGMANAGER_H
