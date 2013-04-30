#include "configmanager.h"
#include "Utility.h"




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
        qDebug() << "[ConfigManager] Invalid conversion of"
                 << m_Container[key]
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
