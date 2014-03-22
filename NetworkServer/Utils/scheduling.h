#ifndef SCHEDULING_H
#define SCHEDULING_H


#include <memory>
#include <functional>
#include <vector>

#include <QObject>
#include <QMutex>

#include <logger.h>

#include <stdexcept>

template<class ChosenType, class Iterable>
class BalancingStrategy
{
public:
    virtual ChosenType choose() = 0;
};

template<class ChosenType, class Iterable>
class RoundRobin : public BalancingStrategy<ChosenType, Iterable>
{
    const Iterable& _iterable;
    typename Iterable::const_iterator it;
    QMutex _lock;
public:
    RoundRobin(const Iterable&);
    // Thread-safe
    ChosenType choose();
};

template<class T, class Balanced>
class Balancer
{
    typedef BalancingStrategy<T, std::vector<Balanced> > StrategyType;
    std::unique_ptr<StrategyType> _balancing_strategy;
    std::function<void(T, Balanced)> _distribute_adapter;
    std::vector<Balanced> _pool;

public:
    Balancer(std::unique_ptr<StrategyType > balancing_strategy,
             std::function<void(T, Balanced)> distribute_adapter,
             std::vector<Balanced>& pool);

    void distribute(T obj);
};


template<class ChosenType, class Iterable>
RoundRobin<ChosenType, Iterable>::RoundRobin(const Iterable& iterable)
    : _iterable(iterable)
{
    it = _iterable.begin();
    if(it == _iterable.end())
    {
        throw std::invalid_argument("Iterable is empty.");
    }
}


template<class ChosenType, class Iterable>
ChosenType RoundRobin<ChosenType, Iterable>::choose()
{
    _lock.lock();
    if(it == _iterable.end())
    {
        it = _iterable.begin();
    }
    auto value = *it;

    it++;

    _lock.unlock();
    return std::move(value);
}

template<class T, class Balanced>
Balancer<T, Balanced>::Balancer(std::unique_ptr<StrategyType> balancing_strategy,
                                std::function<void (T, Balanced)> distribute_adapter,
                                std::vector<Balanced>& pool)
    :  _balancing_strategy(move(balancing_strategy)),
      _distribute_adapter(distribute_adapter),
      _pool(pool)
{

}

template<class T, class Balanced>
void Balancer<T, Balanced>::distribute(T obj)
{
    auto target = std::move(_balancing_strategy->choose());
    distribute_adapter(std::move(obj), std::move(target));
}


#endif // SCHEDULING_H
