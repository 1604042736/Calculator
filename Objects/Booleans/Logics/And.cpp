#include "And.h"
#include "Or.h"

boolptr_t And::operator&&(And b)
{
    boolptr_t result(dynamic_cast<Boolean *>(this->copyThis()));
    for (size_t i = 0; i < b.args.size(); i++)
        result = result && b.args[i];
    return result;
}

boolptr_t And::operator&&(boolptr_t b)
{
    if (isinstance<And>(b))
        return (*this) && *(And *)b.get();
    bool flag = false;
    logicargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        boolptr_t t = this->args[i] && b;
        if (flag || isinstance<And>(t)) // 之前已经合并过或者不可以合并
            args.push_back(this->args[i]);
        else
        {
            flag = true;
            args.push_back(t);
        }
    }
    if (!flag)
        args.push_back(b);
    return boolptr_t(new And(args));
}

boolptr_t And::operator!()
{
    // 反演律
    logicargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
        args.push_back(!this->args[i]);
    return boolptr_t(new Or(args));
}

boolptr_t And::_simplify()
{
    boolptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result && this->args[i]->simplify();
    return result;
}