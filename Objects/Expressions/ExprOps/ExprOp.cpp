#include <algorithm>
#include <cstring>

#include "ExprOp.h"
#include "Add.h"
#include "Mul.h"
#include "Integer.h"
#include "Pow.h"
#include "Common.h"

ExprOp::ExprOp(expropargs_t args)
{
    this->args = args;
}

boolptr_t ExprOp::operator==(exprptr_t b)
{
    if (this->isBaseclass(b))
    {
        ExprOp *a = this;
        ExprOp *c = (ExprOp *)b.get();
        a->sortArgs();
        c->sortArgs();
        expropargs_t aargs(a->args);
        expropargs_t cargs(c->args);
        if (aargs.size() != cargs.size())
            return to_boolean(false);
        if (a->isCommutative())
        {
            for (size_t i = 0; i < aargs.size(); i++)
            {
                bool flag = false;
                for (size_t j = 0; j < cargs.size(); j++)
                {
                    if (isinstance<True>(aargs[i] == cargs[j]))
                    {
                        cargs.erase(cargs.begin() + j); // 防止重复查找
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                    return to_boolean(false);
            }
        }
        else
        {
            for (size_t i = 0; i < aargs.size(); i++)
            {
                if (isinstance<True>(aargs[i] != cargs[i]))
                    return to_boolean(false);
            }
        }
        return to_boolean(true);
    }
    return Expression::operator==(b);
}

std::string ExprOp::toString()
{
    std::string result = "";
    for (size_t i = 0; i < this->args.size(); i++)
    {
        if (this->args[i]->getPriority() < this->getPriority())
            result += " ( " + this->args[i]->toString() + " ) ";
        else
            result += this->args[i]->toString();
        if (i != this->args.size() - 1)
            result += " " + this->getOpStr() + " ";
    }
    return result;
}

prettystring_t ExprOp::toPrettyString()
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
        if (typeid(*this->args[i].get()) == typeid(Pow))
        {
            if ((max_height - 1) / 2 < args_str[i].size() - 1)
            {
                size_t d = (args_str[i].size() - 1) - (max_height - 1) / 2;
                for (size_t j = 0; j < d; j++)
                    result.insert(result.begin(), std::string(result.back().size(), ' '));
                max_height += d;
            }
            y = (max_height - 1) / 2 - (args_str[i].size() - 1);
        }

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
            result[(max_height - 1) / 2] += " " + this->getOpStr() + " ";
        result = normalize(result);
    }

    return normalize(result);
}

exprptr_t ExprOp::operator+(exprptr_t b)
{
    if (isinstance<True>(*this == b))
        return this->operator*(Integer(2));
    return Expression::operator+(b);
}

/*对参数进行整理*/
void ExprOp::sortArgs()
{
    if (this->isCommutative())
    {
        // 合并同类的
        bool changed = true;
        while (changed)
        {
            changed = false;
            expropargs_t args;
            for (size_t i = 0; i < this->args.size(); i++)
            {
                if (this->isBaseclass(this->args[i]))
                {
                    ExprOp *t = (ExprOp *)this->args[i].get(); // 可以保证this->args[i]一定是ExprOp
                    for (size_t j = 0; j < t->args.size(); j++)
                        args.push_back(t->args[j]);
                    changed = true;
                }
                else
                    args.push_back(this->args[i]);
            }
            this->args = args;
        }

        std::sort(this->args.begin(), this->args.end(),
                  [](exprptr_t &a, exprptr_t &b)
                  {
                      return a->getWeight() < b->getWeight();
                  });
    }
}

exprptr_t ExprOp::simplify()
{
    this->sortArgs();
    return Expression::simplify();
}

objptr_t ExprOp::replace(objptr_t old, objptr_t _new)
{
    ExprOp *o = dynamic_cast<ExprOp *>(this->copyThis());
    for (size_t i = 0; i < o->args.size(); i++)
        // 假设替换完后一定为Expression
        o->args[i] = dynamic_cast<Expression *>(o->args[i]->replace(old, _new).get())->copyToExprPtr();
    return o->simplify();
}