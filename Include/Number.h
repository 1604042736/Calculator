#pragma once

#include "Expression.h"
#include "EnumSet.h"

/*数字*/
class Number : virtual public Expression
{
public:
    /*转换成double类型, 与eval不一样*/
    virtual double toDouble() = 0;
    virtual setptr_t belongto() { return setptr_t(new EnumSet({this->copyToExprPtr()})); }
};