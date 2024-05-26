#pragma once

#include "Expression.h"
#include "Integer.h"

typedef std::vector<std::pair<exprptr_t, Integer>> difftargets_t;

/*导数*/
class Derivative : public Expression
{
public:
    Derivative(exprptr_t expr, difftargets_t targets) : expr(expr), targets(targets) {}

    virtual Object *copyThis() { return new Derivative(*this); }

    exprptr_t expr;        // 要求导的表达式
    difftargets_t targets; // 求导对象
};