#ifndef SCHEDULING_H
#define SCHEDULING_H


#include <memory>
#include <functional>
#include <vector>

#include <QObject>
#include <QMutex>

#include <logger.h>

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


#endif // SCHEDULING_H
