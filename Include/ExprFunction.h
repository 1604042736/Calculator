#pragma once

#include "Expression.h"
#include "Function.h"
#include "Mapping.h"

/*可以作为表达式的函数*/
class ExprFunction : public Function, public Expression
{
public:
    ExprFunction(std::string name, funcargs_t args) : Function(name, args) {}
    ExprFunction(mappingptr_t mapping, funcargs_t args) : Function(mapping, args) {}

    virtual Object *copyThis() { return new ExprFunction(*this); }

    virtual boolptr_t operator==(exprptr_t);
    virtual boolptr_t operator==(objptr_t b) { return Expression::operator==(b); }

    virtual exprptr_t diff(exprptr_t);

    virtual objptr_t _simplify_() { return Expression::simplify(); }
};