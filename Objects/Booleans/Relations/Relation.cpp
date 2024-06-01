#include "Relation.h"
#include "Common.h"

prettystring_t Relation::toPrettyString()
{
    prettystring_t lhs_str = this->lhs->toPrettyString();
    prettystring_t rhs_str = this->rhs->toPrettyString();

    size_t max_len = std::max(lhs_str.size(), rhs_str.size());
    prettystring_t result(max_len, "");

    size_t y = (max_len - lhs_str.size()) / 2;
    for (size_t i = 0; i < lhs_str.size(); i++, y++)
        result[y] += lhs_str[i];
    result = normalize(result);

    for (size_t i = 0; i < result.size(); i++)
    {
        if (i == max_len / 2)
            result[i] += " " + this->getRelationStr() + " ";
        else
            result[i] += "   ";
    }

    y = (max_len - rhs_str.size()) / 2;
    for (size_t i = 0; i < rhs_str.size(); i++, y++)
        result[y] += rhs_str[i];

    return normalize(result);
}

bool Relation::operator==(boolptr_t b)
{
    if (this->isSubclass(b))
    {
        Relation *x = this;
        Relation *y = (Relation *)b.get();
        if (x->lhs == y->lhs && x->rhs == y->rhs)
            return true;
        return false;
    }
    return Boolean::operator==(b);
}