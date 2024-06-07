#include <stdexcept>

#include "Log.h"
#include "True.h"
#include "Integer.h"
#include "Interval.h"
#include "Infinity.h"
#include "DescribeSet.h"
#include "Tuple.h"
#include "Lambda.h"
#include "ExprSymbol.h"
#include "Common.h"
#include "Ln.h"

Log::Log(exprptr_t base, exprptr_t tnum) : base(base), tnum(tnum)
{
    exprptr_t _0(new Integer(0));
    exprptr_t _1(new Integer(1));
    exprptr_t inf(new Infinity());
    if (isinstance<True>(tnum < _0) || isinstance<True>(base < _0))
        throw std::runtime_error("超出定义域");
    //{(a,x)|a in (0,+oo) && b in R}
    this->domain = Interval(_0, inf, true, true).product(RealSet());
}

std::string Log::toString()
{
    return "log(" + this->base->toString() + "," + this->tnum->toString() + ")";
}

exprptr_t Log::diff(exprptr_t target)
{
    return (this->tnum * Ln(this->base).simplify())->reciprocal() * this->tnum->diff(target);
}

exprptr_t Log::_simplify()
{
    if (isinstance<Pow>(this->tnum))
    {
        Pow *p = dynamic_cast<Pow *>(this->tnum.get());
        std::shared_ptr<Log> l(dynamic_cast<Log *>(this->copyThis()));
        l->tnum = p->getBase();
        return p->getExp() * l;
    }
    else if (isinstance<True>(this->base == this->tnum))
        return exprptr_t(new Integer(1));
    return ExprFunction::_simplify();
}

boolptr_t Log::operator==(exprptr_t b)
{
    if (isinstance<Log>(b))
    {
        Log *c = dynamic_cast<Log *>(b.get());
        return this->base == c->base && this->tnum == c->tnum;
    }
    return to_boolean(false);
}

objptr_t Log::replace(objptr_t old, objptr_t _new)
{
    objptr_t base = this->base->replace(old, _new);
    objptr_t tnum = this->tnum->replace(old, _new);
    if (!isinstance<Expression>(base) || !isinstance<Expression>(tnum))
        throw std::runtime_error("[Log::replace]超出定义域");
    return exprptr_t(new Log(dynamic_cast<Expression *>(base.get())->copyToExprPtr(),
                             dynamic_cast<Expression *>(tnum.get())->copyToExprPtr()));
}

LogMapping::LogMapping()
    : ExprMapping("log", setptr_t(new RealSet()), setptr_t(new RealSet()))
{
    exprptr_t _0(new Integer(0));
    exprptr_t _1(new Integer(1));
    exprptr_t inf(new Infinity());
    //{(a,x)|a in (0,+oo) && x in R}
    this->domain = Interval(_0, inf, true, true).product(RealSet());
}

exprptr_t LogMapping::operator()(exprptr_t base, exprptr_t tnum)
{
    return exprptr_t(new Log(base, tnum));
}

objptr_t LogMapping::operator()(funcargs_t args)
{
    if (args.size() != 2)
        throw std::runtime_error("超出定义域");
    Expression *base = dynamic_cast<Expression *>(args[0].get());
    Expression *tnum = dynamic_cast<Expression *>(args[1].get());
    if (tnum == nullptr || base == nullptr || isinstance<False>(base->copyToExprPtr() > Integer(0)))
        throw std::runtime_error("超出定义域");
    return this->operator()(base->copyToExprPtr(), tnum->copyToExprPtr());
}