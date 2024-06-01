#include "ProductSet.h"
#include "Common.h"

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