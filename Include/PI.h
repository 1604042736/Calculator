#pragma once

#include "Object.h"
#include "Expression.h"
#include "Number.h"
#include "Mul.h"
#include "Pow.h"
#include "Integer.h"

class Integer;
/*圆周率*/
class PI : public Number
{
public:
    virtual std::string toString() { return "π"; }
    virtual std::string toLateX() { return "\\pi"; }

    virtual bool operator==(PI) { return true; }

    virtual Mul operator+(PI) { return Mul({this->copyToExprPtr(), exprptr_t(new Integer(2))}); }
    virtual Integer operator-(PI) { return 0; }
    virtual Pow operator*(PI) { return Pow({this->copyToExprPtr(), exprptr_t(new Integer(2))}); }
    virtual Integer operator/(PI) { return 1; }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator/(exprptr_t);

    virtual boolptr_t operator==(exprptr_t);

    virtual exprptr_t eval(Integer);

    virtual Object *copyThis() { return new PI(*this); }

    virtual setptr_t belongto() { return setptr_t(new RealSet()); }
};