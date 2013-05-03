#include "configmanager.h"


template<>
QString ConfigManager<>::operator[](const QString& key) const
{
    QString keyLower = key.toLower();
    if(!m_Container.contains(keyLower))
    {
        qDebug() << "[ConfigManager] Key " << key << "has not been found.";
        throw KeyNotFound();
    }
    return m_Container[keyLower];
}

template<>
int ConfigManager<>::intAt(const QString& key) const
{
    // It's nothing by default.

    QString hasKey = (*this)[key.toLower()];

    bool ok;
    int val = hasKey.toInt(&ok);

    if(!ok)
    {
        qDebug() << "[ConfigManager] Invalid conversion of"
                 << m_Container[key] << "to int for key"
                 << key << ".";
        throw InvalidValue();
    }
    return val;
}


template<>
bool ConfigManager<>::contains(const QString& key) const
{
    return m_Container.contains(key.toLower());
}
