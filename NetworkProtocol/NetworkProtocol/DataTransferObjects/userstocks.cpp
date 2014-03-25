#include "userstocks.h"


namespace NetworkProtocol
{
namespace DTO
{

using namespace std;

UserStocks::UserStocks()
{
    _userStocks.reset(new StockList());
}


void UserStocks::addStock(DTO::Types::StockIdType stockId,
                          DTO::Types::AmountType amount)
{
    _userStocks->push_back(qMakePair(stockId, amount));
}

shared_ptr<UserStocks::StockList> UserStocks::getUserStocks() const
{
    return _userStocks;
 }

Types::Message::MessageLengthType UserStocks::lengthSerialized() const
{
    return _userStocks->size() * (sizeof(Types::StockIdType) + sizeof(Types::AmountType));
}


}
}
