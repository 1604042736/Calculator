#include "Or.h"
#include "And.h"

boolptr_t Or::operator||(Or b)
{
    boolptr_t result(dynamic_cast<Boolean *>(this->copyThis()));
    for (size_t i = 0; i < b.args.size(); i++)
        result = result || b.args[i];
    return result;
}

boolptr_t Or::operator||(boolptr_t b)
{
    if (typeid(*b.get()) == typeid(Or))
        return (*this) || *(Or *)b.get();
    bool flag = false;
    logicargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
    {
        boolptr_t t = this->args[i] || b;
        if (flag || typeid(*t.get()) == typeid(Or)) // 之前已经合并过或者不可以合并
            args.push_back(this->args[i]);
        else
        {
            flag = true;
            args.push_back(t);
        }
    }
    if (!flag)
        args.push_back(b);
    return boolptr_t(new Or(args));
}

boolptr_t Or::operator!()
{
    // 反演律
    logicargs_t args;
    for (size_t i = 0; i < this->args.size(); i++)
        args.push_back(!this->args[i]);
    return boolptr_t(new And(args));
}

boolptr_t Or::_simplify()
{
    boolptr_t result = this->args[0]->simplify();
    for (size_t i = 1; i < this->args.size(); i++)
        result = result || this->args[i]->simplify();
    return result;
}