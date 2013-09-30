#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{
namespace Types
{

OrderType toOrderType(qint8 type)
{
    switch(type)
    {
    case BUY: return BUY;
    case SELL: return SELL;
    default: return UNDEFINED;
    };
}

}
}
}
