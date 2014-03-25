#ifndef USERSTOCKS_H
#define USERSTOCKS_H

#include "networkprotocol_global.h"
#include "types.h"

#include <memory>

namespace NetworkProtocol
{
namespace DTO
{

class NETWORKPROTOCOLSHARED_EXPORT UserStocks
{
    typedef QVector<QPair<DTO::Types::StockIdType, DTO::Types::AmountType> > StockList;
    std::shared_ptr<StockList> _userStocks;
public:
    UserStocks();

    void addStock(DTO::Types::StockIdType stockId, DTO::Types::AmountType amount);
    std::shared_ptr<StockList> getUserStocks() const;

    Types::Message::MessageLengthType lengthSerialized() const;

};

// NETWORKPROTOCOLSHARED_EXPORT QDataStream& operator<<(QDataStream&, const UserStocks&);

}
}
#endif // USERSTOCKS_H
