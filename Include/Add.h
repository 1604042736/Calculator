#pragma once

#include "Object.h"
#include "ExprOp.h"

/*加法运算*/
class Add : public ExprOp
{
public:
    Add(expropargs_t args) : ExprOp(args) { this->sortArgs(); }

    virtual Object *copyThis() { return new Add(*this); }

    virtual inline std::string getOpStr() { return "+"; }
    virtual inline bool isCommutative() { return true; }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual exprptr_t operator+(Add);
    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t opposite();
    virtual exprptr_t pow(Integer);

    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t diff(exprptr_t);

    virtual exprptr_t getCoef(exprptr_t);

    virtual exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);

    Integer getWeight();

    virtual size_t getPriority() { return 1; }

    virtual setptr_t belongto();

    virtual bool isBaseclass(objptr_t b) { return isinstance<Add>(b); }
};