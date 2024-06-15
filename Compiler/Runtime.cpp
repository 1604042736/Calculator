#include <stdexcept>
#include <iostream>

#include "Runtime.h"
#include "Sqrt.h"

#include "PI.h"
#include "E.h"
#include "Log.h"
#include "Infinity.h"
#include "True.h"
#include "False.h"
#include "UniversalSet.h"
#include "RealSet.h"
#include "RationalSet.h"
#include "IntegerSet.h"
#include "EmptySet.h"
#include "Abs.h"
#include "Ln.h"
#include "Lg.h"
#include "Sin.h"
#include "Cos.h"
#include "Tan.h"
#include "Function.h"
#include "SetFunction.h"
#include "SetSymbol.h"
#include "SetMapping.h"
#include "ExprFunction.h"
#include "Tuple.h"

class Simplify : public Function
{
public:
    Simplify(objptr_t);

    virtual Object *copyThis() { return new Simplify(*this); }

    virtual objptr_t _simplify_() { return ::simplify(arg); }

    objptr_t arg;
};
class SimplifyMapping : public Mapping
{
public:
    SimplifyMapping() : Mapping("simplify") {}

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1)
            throw std::runtime_error("[Simplify]超出定义域");
        return objptr_t(new Simplify(args[0]));
    }
};

Simplify::Simplify(objptr_t arg) : Function("simplify", {arg})
{
    this->mapping = mappingptr_t(new SimplifyMapping());
}

class Eval : public ExprFunction
{
public:
    Eval(exprptr_t, Integer);

    virtual Object *copyThis() { return new Eval(*this); }

    virtual exprptr_t _simplify()
    {
        return target->eval(keep);
    }
    exprptr_t target;
    Integer keep;
};
class EvalMapping : public ExprMapping
{
public:
    EvalMapping() : ExprMapping("eval", {}, RealSet().product(IntegerSet()), setptr_t(new RationalSet())) {}

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 2 || !isinstance<Expression>(args[0]) || !isinstance<Integer>(args[1]))
            throw std::runtime_error("[Eval]超出定义域");
        exprptr_t target = dynamic_cast<Expression *>(args[0].get())->copyToExprPtr();
        Integer keep = *dynamic_cast<Integer *>(args[1].get());
        return objptr_t(new Eval(target, keep));
    }
};

Eval::Eval(exprptr_t target, Integer keep)
    : target(target), keep(keep), ExprFunction("eval", {target, keep.copyToExprPtr()})
{
    this->mapping = mappingptr_t(new EvalMapping());
}

class Print : public Function
{
public:
    Print(funcargs_t);

    virtual Object *copyThis() { return new Print(*this); }

    virtual objptr_t _simplify_()
    {
        for (size_t i = 0; i < args.size(); i++)
        {
            objptr_t arg = ::simplify(args[i]);
            if (arg == nullptr)
                continue;
            print(arg->toPrettyString());
        }
        return nullptr;
    }
};
class PrintMapping : public Mapping
{
public:
    PrintMapping() : Mapping("print") {}

    virtual objptr_t operator()(funcargs_t args) { return objptr_t(new Print(args)); }
};

Print::Print(funcargs_t args) : Function("print", args)
{
    this->mapping = mappingptr_t(new PrintMapping());
}

class Diff : public ExprFunction
{
public:
    Diff(std::vector<exprptr_t>);

    virtual Object *copyThis() { return new Diff(*this); }

    virtual exprptr_t _simplify()
    {
        exprptr_t result = args[0]->simplify()->diff(args[1]);
        for (size_t i = 2; i < args.size(); i++)
            result = result->diff(args[i]);
        return result;
    }

    std::vector<exprptr_t> args;
};
class DiffMapping : public ExprMapping
{
public:
    DiffMapping() : ExprMapping("diff", {}, setptr_t(new UniversalSet()), setptr_t(new RealSet())) {}

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() <= 1)
            throw std::runtime_error("[Diff]超出定义域");
        std::vector<exprptr_t> e_args;
        for (size_t i = 0; i < args.size(); i++)
        {
            if (!isinstance<Expression>(args[i]))
                throw std::runtime_error("[Diff]超出定义域");
            e_args.push_back(dynamic_cast<Expression *>(args[i].get())->copyToExprPtr());
        }
        return objptr_t(new Diff(e_args));
    }
};

Diff::Diff(std::vector<exprptr_t> args)
    : args(args), ExprFunction("diff", funcargs_t(args.begin(), args.end()))
{
    this->mapping = mappingptr_t(new DiffMapping());
}

class Factorint : public SetFunction
{
public:
    Factorint(Integer);

    virtual Object *copyThis() { return new Factorint(*this); }

    virtual setptr_t _simplify()
    {
        Integer n = arg;
        if (n < 2)
            throw std::runtime_error("[Factorint]超出定义域");
        elements_t elements;
        for (Integer i = 2; n != 1; i = i + 1)
        {
            Integer t = 0;
            while (n % i == 0)
            {
                n = floordiv(n, i);
                t = t + 1;
            }
            if (t == 1)
                elements.push_back(i.copyToExprPtr());
            else if (t > 1)
                elements.push_back(objptr_t(new Tuple({i.copyToExprPtr(), t.copyToExprPtr()})));
        }
        return setptr_t(new EnumSet(elements));
    }

    Integer arg;
};
class FactorintMapping : public SetMapping
{
public:
    FactorintMapping()
        : SetMapping("factorint",
                     {},
                     Interval(exprptr_t(new Integer(2)), exprptr_t(new Infinity()), false, true) & setptr_t(new IntegerSet()),
                     setptr_t(new EnumSet({setptr_t(new UniversalSet())}))) {}

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() != 1 || !isinstance<Integer>(args[0]))
            throw std::runtime_error("[Factorint]超出定义域");
        return objptr_t(new Factorint(*dynamic_cast<Integer *>(args[0].get())));
    }
};

Factorint::Factorint(Integer arg) : arg(arg), SetFunction("factorint", {arg.copyToExprPtr()})
{
    this->mapping = mappingptr_t(new FactorintMapping());
}

class ProductSetFunction : public SetFunction
{
public:
    ProductSetFunction(std::vector<setptr_t>);

    virtual Object *copyThis() { return new ProductSetFunction(*this); }

    virtual setptr_t _simplify()
    {
        if (args.size() == 0)
            throw std::runtime_error("[ProductSetFunction]超出定义域");
        setptr_t result = args[0];
        for (size_t i = 1; i < args.size(); i++)
            result = result->product(args[i]);
        return result;
    }

    std::vector<setptr_t> args;
};
class ProductSetMapping : public SetMapping
{
public:
    ProductSetMapping() : SetMapping("productset") {}

    virtual objptr_t operator()(funcargs_t args)
    {
        if (args.size() == 0)
            throw std::runtime_error("[ProductSetFunction]超出定义域");
        std::vector<setptr_t> set_args;
        for (size_t i = 0; i < args.size(); i++)
        {
            if (!isinstance<Set>(args[i]))
                throw std::runtime_error("[ProductSetFunction]超出定义域");
            set_args.push_back(dynamic_cast<Set *>(args[i].get())->copyToSetPtr());
        }
        return objptr_t(new ProductSetFunction(set_args));
    }
};

ProductSetFunction::ProductSetFunction(std::vector<setptr_t> args)
    : args(args), SetFunction("productset", funcargs_t(args.begin(), args.end()))
{
    this->mapping = mappingptr_t(new ProductSetMapping());
}

Runtime::Runtime()
{
    this->scopes.push_back({
        {"simplify", objptr_t(new SimplifyMapping())},
        {"eval", objptr_t(new EvalMapping())},
        {"print", objptr_t(new PrintMapping())},
        {"sqrt", objptr_t(new SqrtMapping())},
        {"log", objptr_t(new LogMapping())},
        {"e", objptr_t(new E())},
        {"pi", objptr_t(new PI())},
        {"oo", objptr_t(new Infinity())},
        {"True", objptr_t(new True())},
        {"False", objptr_t(new False())},
        {"diff", objptr_t(new DiffMapping())},
        {"U", objptr_t(new UniversalSet())},
        {"R", objptr_t(new RealSet())},
        {"Q", objptr_t(new RationalSet())},
        {"Z", objptr_t(new IntegerSet())},
        {"factorint", objptr_t(new FactorintMapping())},
        {"productset", objptr_t(new ProductSetMapping())},
        {"abs", objptr_t(new AbsMapping())},
        {"ln", objptr_t(new LnMapping())},
        {"lg", objptr_t(new LgMapping())},
        {"sin", objptr_t(new SinMapping())},
        {"cos", objptr_t(new CosMapping())},
        {"tan", objptr_t(new TanMapping())},
    });
}

/*从当前作用域开始一层一层往上找name, 找不到返回nullptr*/
objptr_t Runtime::findName(std::string name)
{
    for (size_t i = 0; i < this->scopes.size(); i++)
    {
        if (this->scopes[i].find(name) != this->scopes[i].end())
            return this->scopes[i][name];
    }
    return nullptr;
}

/*在当前作用域定义name*/
void Runtime::defName(std::string name, objptr_t val)
{
    this->scopes[0][name] = val;
}

void Runtime::enterScope()
{
    this->scopes.insert(this->scopes.begin(), scope_t());
}

void Runtime::leaveScope()
{
    this->scopes.erase(this->scopes.begin());
}

scope_t &Runtime::curScope()
{
    return this->scopes[0];
}