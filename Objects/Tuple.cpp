#include "Tuple.h"
#include "Boolean.h"
#include "Common.h"

std::string Tuple::toString()
{
    std::string result = "(";
    for (size_t i = 0; i < this->args.size(); i++)
        result += this->args[i]->toString() + ", ";
    result += ")";
    return result;
}

boolptr_t Tuple::operator==(Tuple b)
{
    if (this->args.size() != b.args.size())
        return to_boolean(false);
    if (this->args.size() == 0 && b.args.size() == 0)
        return to_boolean(true);
    boolptr_t result = this->args[0] == b.args[0];
    for (size_t i = 1; i < this->args.size(); i++)
        result = result && this->args[i] == b.args[i];
    return result;
}

boolptr_t Tuple::operator==(objptr_t b)
{
    if (isinstance<Tuple>(b))
        return *this == *dynamic_cast<Tuple *>(b.get());
    return to_boolean(false);
}