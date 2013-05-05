#include "configmanager.h"


template<>
QString ConfigManager<>::operator[](const QString& key) const
{
    QString keyLower = key.toLower();
    if(!m_Container.contains(keyLower))
    {
        qDebug() << "[ConfigManager] Klucz " << key
                 << "nie został znaleziony.";
        throw KeyNotFound();
    }
    return m_Container[keyLower];
}

template<>
qint32 ConfigManager<>::intAt(const QString& key) const
{
    QString hasKey = (*this)[key.toLower()];

    bool ok;
    int val = hasKey.toInt(&ok);

    if(!ok)
    {
        qDebug() << "[ConfigManager] Nieudana próba konwersji"
                 << m_Container[key] << "do qint32 dla klucza"
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
