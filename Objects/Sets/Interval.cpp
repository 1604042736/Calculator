#include "Interval.h"

std::string Interval::toString()
{
    std::string result = "";
    if (this->left_open)
        result += "(";
    else
        result += "[";
    result += this->start->toString();
    result += ", ";
    result += this->end->toString();
    if (this->right_open)
        result += ")";
    else
        result += "]";
    return result;
}

prettystring_t Interval::toPrettyString()
{
    prettystring_t result = this->start->toPrettyString();
    result.back() += ", ";
    result = normalize(result);

    prettystring_t end_str = this->end->toPrettyString();
    size_t d = end_str.size() - result.size();
    for (size_t i = 0; i < d; i++)
        result.insert(result.begin(), std::string(result[0].size(), ' '));
    for (size_t i = 0; i < end_str.size(); i++)
        result[i] += end_str[i];

    std::string l, r;
    if (this->left_open)
        l = "(";
    else
        l = "[";
    if (this->right_open)
        r = ")";
    else
        r = "]";

    for (size_t i = 0; i < result.size(); i++)
        result[i] = l + result[i] + r;

    return normalize(result);
}