#include <stdexcept>

#include "BinOpAST.h"
#include "UnaryOpAST.h"
#include "Error.h"
#include "Common.h"
#include "Equality.h"
#include "Add.h"
#include "Mul.h"
#include "Reciprocal.h"
#include "Pow.h"
#include "And.h"
#include "Or.h"
#include "Unequality.h"
#include "LessThan.h"
#include "StrictLessThan.h"
#include "GreaterThan.h"
#include "StrictGreaterThan.h"
#include "Union.h"
#include "Intersection.h"
#include "ProductSet.h"

objptr_t add(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Add({ea, eb}));
    }
    return a->operator+(b);
}

objptr_t sub(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Add({ea, opposite(eb)}));
    }
    return a->operator-(b);
}

objptr_t mul(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Mul({ea, eb}));
    }
    if (isinstance<Set>(a) && isinstance<Set>(b))
    {
        setptr_t ea = dynamic_cast<Set *>(a.get())->copyToSetPtr();
        setptr_t eb = dynamic_cast<Set *>(b.get())->copyToSetPtr();
        return objptr_t(new ProductSet({ea, eb}));
    }
    return a->operator*(b);
}

objptr_t div(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Mul({ea, exprptr_t(new Reciprocal(eb))}));
    }
    return a->operator/(b);
}

objptr_t pow(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Pow({ea, eb}));
    }
    if (isinstance<Set>(a) && isinstance<Integer>(b))
    {
        setptr_t sa = dynamic_cast<Set *>(a.get())->copyToSetPtr();
        Integer ib = *dynamic_cast<Integer *>(b.get());
        return objptr_t(new ProductSet({sa, ib}));
    }
    return a->pow(b);
}

objptr_t _and(objptr_t a, objptr_t b)
{
    if (isinstance<Boolean>(a) && isinstance<Boolean>(b))
    {
        boolptr_t ba = dynamic_cast<Boolean *>(a.get())->copyToBoolPtr();
        boolptr_t bb = dynamic_cast<Boolean *>(b.get())->copyToBoolPtr();
        return boolptr_t(new And({ba, bb}));
    }
    return a->operator&&(b);
}

objptr_t _or(objptr_t a, objptr_t b)
{
    if (isinstance<Boolean>(a) && isinstance<Boolean>(b))
    {
        boolptr_t ba = dynamic_cast<Boolean *>(a.get())->copyToBoolPtr();
        boolptr_t bb = dynamic_cast<Boolean *>(b.get())->copyToBoolPtr();
        return boolptr_t(new Or({ba, bb}));
    }
    return a->operator||(b);
}

objptr_t eq(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Equality(ea, eb));
    }
    return a->operator==(b);
}

objptr_t neq(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new Unequality(ea, eb));
    }
    return a->operator!=(b);
}

objptr_t gt(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new StrictGreaterThan(ea, eb));
    }
    return a->operator>(b);
}

objptr_t ge(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new GreaterThan(ea, eb));
    }
    return a->operator>=(b);
}

objptr_t lt(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new StrictLessThan(ea, eb));
    }
    return a->operator<(b);
}

objptr_t le(objptr_t a, objptr_t b)
{
    if (isinstance<Expression>(a) && isinstance<Expression>(b))
    {
        exprptr_t ea = dynamic_cast<Expression *>(a.get())->copyToExprPtr();
        exprptr_t eb = dynamic_cast<Expression *>(b.get())->copyToExprPtr();
        return objptr_t(new LessThan(ea, eb));
    }
    return a->operator<=(b);
}

objptr_t _bitand(objptr_t a, objptr_t b)
{
    if (isinstance<Set>(a) && isinstance<Set>(b))
    {
        setptr_t ea = dynamic_cast<Set *>(a.get())->copyToSetPtr();
        setptr_t eb = dynamic_cast<Set *>(b.get())->copyToSetPtr();
        return objptr_t(new Intersection({ea, eb}));
    }
    return a->operator&(b);
}

objptr_t _bitor(objptr_t a, objptr_t b)
{
    if (isinstance<Set>(a) && isinstance<Set>(b))
    {
        setptr_t ea = dynamic_cast<Set *>(a.get())->copyToSetPtr();
        setptr_t eb = dynamic_cast<Set *>(b.get())->copyToSetPtr();
        return objptr_t(new Union({ea, eb}));
    }
    return a->operator|(b);
}

objptr_t BinOpAST::exec(Runtime *runtime)
{
    objptr_t l = this->children[0]->exec(runtime);
    objptr_t r = this->children[1]->exec(runtime);
    try
    {
        if (this->op == "+")
            return add(l, r);
        else if (this->op == "-")
            return sub(l, r);
        else if (this->op == "*")
            return mul(l, r);
        else if (this->op == "/")
            return div(l, r);
        else if (this->op == "^")
            return pow(l, r);
        else if (this->op == "and")
            return _and(l, r);
        else if (this->op == "or")
            return _or(l, r);
        else if (this->op == "=")
            return eq(l, r);
        else if (this->op == "!=")
            return neq(l, r);
        else if (this->op == ">")
            return gt(l, r);
        else if (this->op == ">=")
            return ge(l, r);
        else if (this->op == "<")
            return lt(l, r);
        else if (this->op == "<=")
            return le(l, r);
        else if (this->op == "&")
            return _bitand(l, r);
        else if (this->op == "|")
            return _bitor(l, r);
        else
            throw Error("意料之外的二元运算符: " + this->op, this->context);
    }
    catch (std::exception &e)
    {
        throw Error(e.what(), this->context);
    }
}