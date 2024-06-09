#include <iostream>

#include "TrigFunction.h"
#include "PI.h"
#include "True.h"

exprptr_t TrigFunction::_simplify()
{
    TrigFunction *tf = dynamic_cast<TrigFunction *>(this->copyThis());
    exprptr_t pi(new PI());
    exprptr_t coef = tf->arg->getCoef(pi);
    if (can_reduce(coef))
        reducePeriod(tf->arg, coef);
    exprptr_t ret = tf->matchSpecial();
    delete tf;
    return ret;
}

/*根据周期化简*/
void TrigFunction::reducePeriod(exprptr_t &arg, exprptr_t &coef)
{
    exprptr_t pi(new PI());
    while (isinstance<True>(coef > Integer(0)))
    {
        arg = arg - this->period * pi;
        coef = coef - this->period;
    }
    while (isinstance<True>(coef < Integer(0)))
    {
        arg = arg + this->period * pi;
        coef = coef + this->period;
    }
}

/*匹配特殊值*/
exprptr_t TrigFunction::matchSpecial()
{
    for (auto t : this->special)
    {
        if (isinstance<True>(t.first == this->arg))
            return t.second;
    }
    return this->copyToExprPtr();
}