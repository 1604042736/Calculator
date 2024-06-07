#pragma once

#include "ExprOp.h"
#include "Common.h"

/*乘法运算*/
class Mul : public ExprOp
{
public:
    Mul(expropargs_t args) : ExprOp(args) { /*this->sortArgs();*/ }

    virtual Object *copyThis() { return new Mul(*this); }

    virtual exprptr_t getNume();
    virtual exprptr_t getDeno();

    virtual std::string toString();
    virtual prettystring_t toPrettyString();
    virtual std::string toLateX();

    virtual inline std::string getOpStr() { return "*"; }
    virtual inline bool isCommutative() { return true; }

    virtual exprptr_t operator+(Mul);
    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator*(Mul);

    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t reciprocal();
    virtual exprptr_t diff(exprptr_t);

    virtual exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);

    virtual setptr_t belongto();

    Integer getWeight();

    virtual size_t getPriority() { return 2; }

    virtual bool isBaseclass(objptr_t b) { return isinstance<Mul>(b); }
};