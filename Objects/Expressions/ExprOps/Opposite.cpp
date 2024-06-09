#include "Opposite.h"
#include "Common.h"
#include "Integer.h"

std::string Opposite::toString()
{
    return " - " + this->opposite()->toString();
}

prettystring_t Opposite::toPrettyString()
{
    prettystring_t result(this->opposite()->toPrettyString());
    for (size_t i = 0; i < result.size(); i++)
    {
        if (i == result.size() / 2)
            result[i] = " - " + result[i];
        else
            result[i] = "   " + result[i];
    }
    return result;
}

exprptr_t Opposite::_simplify()
{
    return this->opposite()->simplify()->opposite();
}

exprptr_t Opposite::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    return this->opposite()->eval(keep)->opposite();
}