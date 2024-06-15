#include <stdexcept>

#include "Derivative.h"

#include "ExprSymbol.h"
#include "Mul.h"
#include "Pow.h"
#include "Reciprocal.h"

class D : public ExprSymbol
{
public:
    D() : ExprSymbol("d") {}
    virtual Object *copyThis() { return new D(); }
    virtual exprptr_t copyToExprPtr() { return exprptr_t(new D()); }
    virtual Integer getWeight() { return 0; }
};

std::string Derivative::toString()
{
    exprptr_t d(new D());
    exprptr_t deno = nullptr;
    Integer n = 0;
    for (size_t i = 0; i < this->targets.size(); i++)
    {
        n = n + this->targets[i].second;
        exprptr_t t(new Mul({d->pow(this->targets[i].second), this->targets[i].first}));
        if (deno == nullptr)
            deno = t;
        else
            deno = exprptr_t(new Mul({deno, t}));
    }
    exprptr_t nume(new Mul({d->pow(n), this->expr}));
    return exprptr_t(new Mul({nume, exprptr_t(new Reciprocal(deno))}))->toString();
}

prettystring_t Derivative::toPrettyString()
{
    exprptr_t d(new D());
    exprptr_t deno = nullptr;
    Integer n = 0;
    for (size_t i = 0; i < this->targets.size(); i++)
    {
        n = n + this->targets[i].second;
        exprptr_t t(new Mul({d->pow(this->targets[i].second), this->targets[i].first}));
        if (deno == nullptr)
            deno = t;
        else
            deno = exprptr_t(new Mul({deno, t}));
    }
    exprptr_t nume(new Mul({d->pow(n), this->expr}));
    return exprptr_t(new Mul({nume, exprptr_t(new Reciprocal(deno))}))->toPrettyString();
}

boolptr_t Derivative::operator==(exprptr_t b)
{
    if (isinstance<Derivative>(b))
    {
        Derivative *c = dynamic_cast<Derivative *>(b.get());
        if (c->targets.size() != this->targets.size())
            return to_boolean(false);
        boolptr_t result = this->expr == c->expr;
        for (size_t i = 0; i < this->targets.size(); i++)
        {
            boolptr_t first = this->targets[i].first == c->targets[i].first;
            boolptr_t second = to_boolean(this->targets[i].second == c->targets[i].second);
            result = result && (first && second);
        }
        return result;
    }
    return to_boolean(false);
}

objptr_t Derivative::replace(objptr_t old, objptr_t _new)
{
    exprptr_t expr = nullptr;
    {
        objptr_t t = this->expr->replace(old, _new);
        if (!isinstance<Expression>(t))
            throw std::runtime_error("[Derivative::replace]求导表达式不是表达式");
        expr = dynamic_cast<Expression *>(t.get())->copyToExprPtr();
    }
    difftargets_t targets(this->targets);
    for (size_t i = 0; i < this->targets.size(); i++)
    {
        objptr_t t = this->targets[i].first->replace(old, _new);
        if (!isinstance<Expression>(t))
            throw std::runtime_error("[Derivative::replace]求导对象不是表达式");
        this->targets[i].first = dynamic_cast<Expression *>(t.get())->copyToExprPtr();
    }
    return exprptr_t(new Derivative(this->expr, targets));
}

exprptr_t Derivative::diff(exprptr_t target)
{
    exprptr_t result = Expression::diff(target);
    if (!isinstance<Derivative>(result))
        return result;
    difftargets_t targets(this->targets);
    bool flag = true;
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (isinstance<True>(targets[i].first == target))
        {
            targets[i].second = targets[i].second + 1;
            flag = false;
            break;
        }
    }
    if (flag)
        targets.push_back({target, Integer(1)});
    result = exprptr_t(new Derivative(this->expr, targets));
    return result;
}