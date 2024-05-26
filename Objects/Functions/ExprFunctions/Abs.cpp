#include "Abs.h"
#include "Interval.h"
#include "Integer.h"
#include "Infinity.h"

Abs::Abs(exprptr_t arg)
{
    exprptr_t _0(new Integer(0));
    exprptr_t inf(new Infinity());
    this->range = setptr_t(new Interval(_0, inf, false, true));

    this->arg = arg;
}

std::string Abs::toString()
{
    return "|" + this->arg->toString() + "|";
}

prettystring_t Abs::toPrettyString()
{
    prettystring_t result = this->arg->toPrettyString();
    for (size_t i = 0; i < result.size(); i++)
        result[i] = "|" + result[i] + "|";
    return normalize(result);
}