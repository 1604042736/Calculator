#pragma once

#include "Expression.h"
#include "EnumSet.h"

/*数字*/
class Number : public Expression
{
public:
    virtual setptr_t belongto() { return setptr_t(new EnumSet({this->copyToExprPtr()})); }
};