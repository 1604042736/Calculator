#include "Reciprocal.h"

std::string Reciprocal::toString()
{
    return "1/" + this->reciprocal()->toString();
}

prettystring_t Reciprocal::toPrettyString()
{
    prettystring_t result(this->reciprocal()->toPrettyString());
    result.insert(result.begin(), std::string(result[0].size(), '-'));
    result.insert(result.begin(), std::string(result[0].size() / 2, ' '));
    result.front() += "1";
    return normalize(result);
}

std::string Reciprocal::toLateX()
{
    return "\\frac{1}{" + this->reciprocal()->toLateX() + "}";
}

exprptr_t Reciprocal::reciprocal()
{
    // this->args的最后一个为-1
    return exprptr_t(new Pow(expropargs_t(this->args.begin(), this->args.end() - 1)));
}

exprptr_t Reciprocal::_simplify()
{
    return this->reciprocal()->simplify()->reciprocal();
}

exprptr_t Reciprocal::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    return this->reciprocal()->eval(keep)->reciprocal();
}