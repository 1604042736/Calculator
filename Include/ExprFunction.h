#pragma once

#include "Expression.h"
#include "Function.h"
#include "UniversalSet.h"

/*可以作为表达式的函数*/
class ExprFunction : public Function, public Expression
{
public:
    ExprFunction() : Function(setptr_t(new UniversalSet()), setptr_t(new RealSet())) {}
};