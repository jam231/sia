#include "scheduling.h"

#include <stdexcept>



using namespace std;

template<class ChosenType, class Iterable>
RoundRobin<ChosenType, Iterable>::RoundRobin(const Iterable& iterable)
{
    _iterable = iterable;
    it = it.begin();
    if(it == _iterable.end())
    {
        throw invalid_argument("Iterable is empty.");
    }
}


template<class ChosenType, class Iterable>
ChosenType RoundRobin<ChosenType, Iterable>::choose()
{
    _lock.lock();
    if(it == _iterable.end())
    {
        it = it.begin();
    }
    auto value = move(*it);

    it++;

    _lock.unlock();
    return move(value);
}

template<class T, class Balanced>
Balancer<T, Balanced>::Balancer(unique_ptr<StrategyType> balancing_strategy,
                                function<void (T, Balanced)> distribute_adapter,
                                vector<Balanced>& pool)
    :  _balancing_strategy(move(balancing_strategy)),
      _distribute_adapter(distribute_adapter),
      _pool(pool)
{

}

template<class T, class Balanced>
void Balancer<T, Balanced>::distribute(T obj)
{
    auto target = move(_balancing_strategy->choose());
    distribute_adapter(obj, move(target));
}
