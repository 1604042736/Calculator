#pragma once

#include <string>

#include "Expression.h"
#include "Symbol.h"

/*表示算术表达式的符号*/
class ExprSymbol : virtual public Expression, public Symbol
{
public:
    ExprSymbol(std::string name, setptr_t range = setptr_t(new RealSet()));

    virtual Object *copyThis() { return new ExprSymbol(*this); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new ExprSymbol(*this)); }

    virtual exprptr_t operator+(ExprSymbol *);
    virtual exprptr_t operator-(ExprSymbol *);
    virtual exprptr_t operator*(ExprSymbol *);
    virtual exprptr_t operator/(ExprSymbol *);
    /*暂且把名称一样的变量看成一样的*/
    virtual bool operator==(ExprSymbol *b) { return this->name == b->name; }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator/(exprptr_t);
    virtual boolptr_t operator==(exprptr_t);

    virtual setptr_t belongto() { return range; }
    setptr_t range;

    Integer getWeight();
};