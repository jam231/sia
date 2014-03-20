#include "configmanager.h"


template<>
QString ConfigManager<>::operator[](const QString& key) const
{
    QString keyLower = key.toLower();
    if(!_container.contains(keyLower))
    {
        qDebug() << "[ConfigManager] Klucz " << key
                 << "nie został znaleziony.";
        throw KeyNotFound();
    }
    return _container[keyLower];
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
                 << _container[key] << "do qint32 dla klucza"
                 << key << ".";
        throw InvalidValue();
    }
    return val;
}


template<>
bool ConfigManager<>::contains(const QString& key) const
{
    return _container.contains(key.toLower());
}

template<>
QHash<QString, QString> ConfigManager<>::asQHash() const
{
    QHash<QString, QString> hash;

    for(auto it = _container.begin(); it != _container.end(); it++)
    {
        hash[it.key()] = it.value();
    }
    return hash;
}
