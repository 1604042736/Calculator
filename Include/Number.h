#pragma once

#include "Expression.h"

/*数字*/
class Number : public Expression
{
public:
    virtual exprptr_t diff(exprptr_t);
};