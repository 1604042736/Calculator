#pragma once

#include "ExprOp.h"
#include "Common.h"

/*幂运算*/
class Pow : public ExprOp
{
public:
    Pow(expropargs_t args) : ExprOp(args) {}

    virtual Object *copyThis() { return new Pow(*this); }

    virtual inline std::string getOpStr() { return "^"; }

    virtual prettystring_t toPrettyString();
    virtual std::string toLateX();

    virtual exprptr_t operator*(Pow);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t diff(exprptr_t);

    virtual exprptr_t _simplify();
    virtual exprptr_t _eval(Integer);

    setptr_t belongto() { return setptr_t(new RealSet()); }

    Integer getWeight();

    virtual size_t getPriority() { return 3; }
    /*获取底数*/
    exprptr_t inline getBase() { return this->args.front(); }
    /*获取指数*/
    exprptr_t inline getExp() { return this->args.size() == 2 ? this->args[1] : exprptr_t(new Pow(expropargs_t(this->args.begin() + 1, this->args.end()))); }

    virtual bool isSubclass(objptr_t b) { return isinstance<Pow>(b); }
};