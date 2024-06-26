#include <stdexcept>

#include "ProductSet.h"

#include "Tuple.h"
#include "EnumSet.h"

ProductSet::ProductSet(setptr_t set, Integer n) : SetOp({})
{
    if (n < 0)
        throw std::runtime_error("集合的pow运算指数必须>=0");
    if (n == 0)
        return;
    setopargs_t args;
    setptr_t self = set;
    for (Integer i = 0; i < n; i = i + 1)
        args.push_back(self);
    this->args = args;
}

std::string ProductSet::toString()
{
    std::string result = "";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (this->args[i]->getPriority() < this->getPriority() || isinstance<ProductSet>(this->args[i]))
            result += " ( " + this->args[i]->toString() + " ) ";
        else
            result += this->args[i]->toString();
        if (i != this->args.size() - 1)
            result += " " + this->getOpStr() + " ";
    }
    return result;
}

prettystring_t ProductSet::toPrettyString()
{
    std::vector<prettystring_t> args_str;
    size_t max_height = 1;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        prettystring_t s = this->args[i]->toPrettyString();
        args_str.push_back(s);
        max_height = std::max(max_height, s.size());
    }

    prettystring_t result(max_height, "");
    for (size_t i = 0; i < args_str.size(); i++)
    {
        size_t y = (max_height - args_str[i].size()) / 2; // 居中

        bool flag = this->args[i]->getPriority() < this->getPriority() || isinstance<ProductSet>(this->args[i]);

        if (flag)
        {
            for (size_t j = 0; j < result.size(); j++)
                result[j] += " ( ";
        }

        for (size_t j = 0; j < args_str[i].size(); j++, y++)
            result[y] += args_str[i][j];

        if (flag)
        {
            for (size_t j = 0; j < result.size(); j++)
                result[j] += " ) ";
        }

        if (i < args_str.size() - 1)
            result[(max_height - 1) / 2] += " " + this->getOpStr() + " ";
        result = normalize(result);
    }

    return normalize(result);
}

boolptr_t ProductSet::contains(objptr_t element)
{
    Tuple *tuple = dynamic_cast<Tuple *>(element.get());
    if (tuple == nullptr || tuple->args.size() != this->args.size())
        return to_boolean(false);
    boolptr_t result(new False());
    for (size_t i = 0; i < this->args.size(); i++)
        result = result || (tuple->args[i]->operator==(this->args[i]));
    return result;
}

setptr_t ProductSet::_simplify()
{
    if (this->args.size() == 0)
        return setptr_t(new EnumSet({objptr_t(new Tuple({}))}));
    setptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result->product(this->args[i]);
    return result;
}