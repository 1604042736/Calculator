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
    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual boolptr_t operator==(exprptr_t);

    virtual exprptr_t diff(exprptr_t);

    virtual objptr_t replace(objptr_t, objptr_t);

    exprptr_t expr;        // 要求导的表达式
    difftargets_t targets; // 求导对象
};