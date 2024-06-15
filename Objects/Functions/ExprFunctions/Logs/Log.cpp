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
#include "Ln.h"
#include "Lg.h"

Log::Log(exprptr_t base, exprptr_t tnum) : base(base), tnum(tnum), ExprFunction("log", {base, tnum})
{
    this->mapping = mappingptr_t(new LogMapping());
}

std::string Log::toString()
{
    return "log(" + this->base->toString() + "," + this->tnum->toString() + ")";
}

exprptr_t Log::diff(exprptr_t target)
{
    return (Ln(this->tnum) / Ln(this->base))->diff(target);
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

LogMapping::LogMapping()
    : ExprMapping("log", {}, setptr_t(new RealSet()), setptr_t(new RealSet()))
{
    exprptr_t _0(new Integer(0));
    exprptr_t _1(new Integer(1));
    exprptr_t inf(new Infinity());
    //{(a,x)|a in (0,+oo) && x in R}
    this->domain = Interval(_0, inf, true, true).product(RealSet());
}

exprptr_t LogMapping::operator()(exprptr_t base, exprptr_t tnum)
{
    if (isinstance<True>(base == E()))
        return exprptr_t(new Ln(tnum));
    if (isinstance<True>(base == Integer(10)))
        return exprptr_t(new Lg(tnum));
    return exprptr_t(new Log(base, tnum));
}

objptr_t LogMapping::operator()(funcargs_t args)
{
    if (args.size() != 2 || !isinstance<Expression>(args[0]) || !isinstance<Expression>(args[1]))
        throw std::runtime_error("[Log]超出定义域");
    exprptr_t base = dynamic_cast<Expression *>(args[0].get())->copyToExprPtr();
    exprptr_t tnum = dynamic_cast<Expression *>(args[1].get())->copyToExprPtr();
    if (isinstance<False>(base > Integer(0)))
        throw std::runtime_error("[Log]超出定义域");
    return this->operator()(base, tnum);
}