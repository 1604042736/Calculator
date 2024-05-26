#include "Contains.h"

std::string Contains::toString()
{
    return this->element->toString() + " in " + this->set->toString();
}

prettystring_t Contains::toPrettyString()
{
    prettystring_t result = this->element->toPrettyString();
    prettystring_t set_str = this->set->toPrettyString();
    size_t d = result.size() / 2;
    if (set_str.size() > result.size())
    {
        d = (set_str.size() - result.size()) / 2;
        for (size_t i = 0; i < d; i++)
        {
            result.insert(result.begin(), std::string(result[0].size(), ' '));
            result.insert(result.end(), std::string(result[0], ' '));
        }
    }
    result[d] += " in ";
    result = normalize(result);

    for (size_t i = 0; i < set_str.size(); i++)
        result[i] += set_str[i];

    return normalize(result);
}