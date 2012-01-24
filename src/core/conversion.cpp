#include "conversion.h"
#include <sstream>

std::string int2string(int nb)
{
    std::ostringstream oss;
    oss << nb;
    return oss.str();
}
