#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

void Response::sendHeader(QDataStream& out)
{
    out << length() << type();
}

}
}
