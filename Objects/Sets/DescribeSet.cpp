#include <iostream>

#include "DescribeSet.h"

std::string DescribeSet::toString()
{
    std::string result = "{";
    result += this->func.toString();
    result += " | ";
    result += this->condition->toString();
    result += "}";
    return result;
}

prettystring_t DescribeSet::toPrettyString()
{
    prettystring_t result = this->func.toPrettyString();

    prettystring_t condition_str = this->condition->toPrettyString();
    if (condition_str.size() > result.size())
    {
        size_t d = condition_str.size() - result.size();
        for (size_t i = 0; i < d; i++)
            result.insert(result.begin(), std::string(result[0].size(), ' '));
    }

    for (size_t i = 0; i < result.size(); i++)
        result[i] += " | ";

    for (size_t i = 0; i < condition_str.size(); i++)
        result[result.size() - i - 1] += condition_str[i];

    result = normalize(result);

    for (size_t i = 0; i < result.size(); i++)
        result[i] = "{ " + result[i] + " }";

    return normalize(result);
}