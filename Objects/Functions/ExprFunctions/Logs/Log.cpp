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

LogMapping::LogMapping()
    : Mapping("log", setptr_t(new RealSet()), setptr_t(new RealSet()))
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