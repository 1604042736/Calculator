#include "Logic.h"

std::string Logic::toString()
{
    std::string result = "";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        bool flag = this->args[i]->getPriority() < this->getPriority();

        if (flag)
            result += "(";
        result += this->args[i]->toString();
        if (flag)
            result += ")";
        if (i < this->args.size() - 1)
            result += " " + this->getLogicStr() + " ";
    }
    return result;
}

prettystring_t Logic::toPrettyString()
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

        bool flag = this->args[i]->getPriority() < this->getPriority();

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
            result[(max_height - 1) / 2] += " " + this->getLogicStr() + " ";
        result = normalize(result);
    }

    return normalize(result);
}

bool Logic::operator==(boolptr_t b)
{
    if (typeid(*b.get()) == typeid(*this))
    {
        Logic *a = this;
        Logic *c = (Logic *)b.get();
        logicargs_t aargs(a->args);
        logicargs_t cargs(c->args);
        if (aargs.size() != cargs.size())
            return false;
        for (size_t i = 0; i < aargs.size(); i++)
        {
            bool flag = false;
            for (size_t j = 0; j < cargs.size(); j++)
            {
                if (aargs[i] == cargs[j])
                {
                    cargs.erase(cargs.begin() + j); // 防止重复查找
                    flag = true;
                    break;
                }
            }
            if (!flag)
                return false;
        }
        return true;
    }
    return Boolean::operator==(b);
}

/*对参数进行整理*/
void Logic::sortArgs()
{
    // 合并同类的
    bool changed = true;
    while (changed)
    {
        changed = false;
        logicargs_t args;
        for (size_t i = 0; i < this->args.size(); i++)
        {
            if (typeid(*this->args[i].get()) == typeid(*this))
            {
                Logic *t = (Logic *)this->args[i].get();
                for (size_t j = 0; j < t->args.size(); j++)
                    args.push_back(t->args[j]);
                changed = true;
            }
            else
                args.push_back(this->args[i]);
        }
        this->args = args;
    }
}