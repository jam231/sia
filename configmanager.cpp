#include "configmanager.h"
#include "Utility.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
/*
 *  TODO:
 *
 *      - Reduce boilerplatet,
 *      - Settle the matter of error passing style.
 *      - Change code to conform into settled error passing style.
 *      - Comments and doxygen.
 */
template<>
void ConfigManager<>::parseAndBuildFromLines(QStringList lines)
{
    QPair<QString, QString> keyValue;
    foreach(keyValue, parseINI(lines))
    {
        if(m_Container.contains(keyValue.first))
        {
            qDebug() << "[ConfigManager] Key " << keyValue.first
                     << " duplicate found. Replacing with new value.";
        }
        m_Container[keyValue.first] = keyValue.second;
    }
}


template<>
Maybe<QString> ConfigManager<>::operator[](const QString& key) const
{
    QString keyLower = key.toLower();
    if(!m_Container.contains(keyLower))
    {
        qDebug() << "[ConfigManager] Key " << key << "not found";
        return Maybe<QString>();
    }
    return Maybe<QString>(m_Container[keyLower]);
}


template<>
ConfigManager<>::ConfigManager(const QString& pathToFile)
{

    /*
     *  1. Open a file pathToFile
     *  2. Read all it's contents
     *  3. Parse it.
     *  4. Produce a Container containing configuration setup.
     */
    qDebug() << "[ConfigManager] Opening config file " << pathToFile;
    QFile configFile(pathToFile);

    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[ConfigManager] Error: file " << pathToFile
                  << " could not be open.";
        return;
    }

    QTextStream configStream(&configFile);
    const int chunkSizeInLines = 100;
    int linesRead = 0;
    QStringList lines;
    while(!configStream.atEnd())
    {

        lines += configStream.readLine();
        linesRead++;
        if(linesRead == chunkSizeInLines)
        {
            parseAndBuildFromLines(lines);
            linesRead = 0;
            lines.clear();
        }
    }
    parseAndBuildFromLines(lines);
    qDebug() << "[ConfigManager]: Building config is finished.";
    qDebug() << m_Container;
}

template<>
Maybe<int> ConfigManager<>::intAt(const QString& key) const
{
    // It's nothing by default.
    Maybe<QString> hasKey = (*this)[key.toLower()];
    if(!hasKey)
        return Maybe<int>();

    bool ok;
    int val = hasKey.value.toInt(&ok);

    if(!ok)
    {
        qDebug() << "[ConfigManager] Invalid conversion of" << m_Container[key]
                 << "to int for key" << key;
        return Maybe<int>();
    }
    return Maybe<int>(val);
}


template<>
bool ConfigManager<>::contains(const QString& key) const
{
    return m_Container.contains(key.toLower());
}
