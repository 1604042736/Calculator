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
#include "DefinedFunction.h"
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

class SimplifyFunction : public SArgFunction
{
public:
    SimplifyFunction(objptr_t arg)
        : SArgFunction("simplify", arg) {}

    virtual Object *copyThis() { return new SimplifyFunction(*this); }

    virtual objptr_t simplify() { return ::simplify(arg); }
};
typedef SArgFuncMapping<Mapping, Object, SimplifyFunction, Symbol> SimplifyMapping;

class EvalFunction : public MArgExprFunction
{
public:
    EvalFunction(std::vector<exprptr_t> args)
        : MArgExprFunction("eval", args, RealSet().product(IntegerSet()), setptr_t(new RationalSet()))
    {
    }

    virtual Object *copyThis() { return new EvalFunction(*this); }

    virtual exprptr_t _simplify()
    {
        if (args.size() != 2 || !isinstance<Integer>(args[1]))
            throw std::runtime_error("[EvalFunction]超出定义域");
        return args[0]->eval(*dynamic_cast<Integer *>(args[1].get()));
    }
};
typedef MArgFuncMapping<ExprMapping, Expression, EvalFunction, ExprSymbol> EvalMapping;

class PrintFunction : public MArgFunction
{
public:
    PrintFunction(std::vector<objptr_t> args)
        : MArgFunction("print",
                       args,
                       setptr_t(new UniversalSet()),
                       setptr_t((new EmptySet()))) {}

    virtual Object *copyThis() { return new PrintFunction(*this); }

    virtual objptr_t simplify()
    {
        for (size_t i = 0; i < args.size(); i++)
        {
            objptr_t arg = ::simplify(args[i]);
            if (arg == nullptr)
                continue;
            print(arg->toPrettyString());
            if (isinstance<DefinedFunction>(arg))
            {
                DefinedFunction *f = dynamic_cast<DefinedFunction *>(arg.get());
                std::cout << "||\n";
                std::cout << "\\/\n";
                for (size_t j = 0; j < f->sections->size(); j++)
                    print((*f->sections)[j].toPrettyString());
            }
        }
        return nullptr;
    }
};
typedef MArgFuncMapping<Mapping, Object, PrintFunction, Symbol> PrintMapping;

class DiffFunction : public MArgExprFunction
{
public:
    DiffFunction(std::vector<exprptr_t> args)
        : MArgExprFunction("diff", args, setptr_t(new RealSet()), setptr_t(new RealSet())) {}

    virtual Object *copyThis() { return new DiffFunction(*this); }

    virtual exprptr_t _simplify()
    {
        if (args.size() < 2)
            throw std::runtime_error("参数不够");
        std::vector<exprptr_t> expr_args;
        for (size_t i = 0; i < args.size(); i++)
        {
            if (!isinstance<Expression>(args[i]))
                throw std::runtime_error("超出定义域");
            expr_args.push_back(dynamic_cast<Expression *>(args[i].get())->copyToExprPtr());
        }
        exprptr_t result = expr_args[0]->diff(expr_args[1]);
        for (size_t i = 2; i < expr_args.size(); i++)
            result = result->diff(expr_args[i]);
        return result;
    }
};
typedef MArgFuncMapping<ExprMapping, Expression, DiffFunction, ExprSymbol> DiffMapping;

class FactorintFunction : public SArgExprFunction
{
public:
    FactorintFunction(exprptr_t arg)
        : SArgExprFunction("factorint", arg, setptr_t(new RealSet()), setptr_t(new RealSet()))
    {
        this->domain = Interval(exprptr_t(new Integer(2)), exprptr_t(new Infinity()), false, true) & setptr_t(new IntegerSet());
        this->range = this->domain;
    }

    virtual Object *copyThis() { return new FactorintFunction(*this); }

    virtual exprptr_t simplify()
    {
        if (!isinstance<Integer>(arg))
            throw std::runtime_error("超出定义域");
        Integer n = *dynamic_cast<Integer *>(arg.get());
        if (n < 2)
            throw std::runtime_error("超出定义域");
        expropargs_t exprop_args;
        for (Integer i = 2; n != 1; i = i + 1)
        {
            Integer t = 0;
            while (n % i == 0)
            {
                n = floordiv(n, i);
                t = t + 1;
            }
            if (t == 1)
                exprop_args.push_back(i.copyToExprPtr());
            else if (t > 1)
                exprop_args.push_back(exprptr_t(new Pow({i.copyToExprPtr(), t.copyToExprPtr()})));
        }
        return exprptr_t(new Mul(exprop_args));
    }
};
typedef SArgFuncMapping<ExprMapping, Expression, FactorintFunction, ExprSymbol> FactorintMapping;

class ProductSetFunction : public MArgSetFunction
{
public:
    ProductSetFunction(std::vector<setptr_t> args)
        : MArgSetFunction("productset", args) {}

    virtual Object *copyThis() { return new ProductSetFunction(*this); }

    virtual setptr_t _simplify()
    {
        if (args.size() == 0)
            throw std::runtime_error("[ProductSetFunction]超出定义域");
        setptr_t result = nullptr;
        for (size_t i = 0; i < args.size(); i++)
        {
            if (!isinstance<Set>(args[i]))
                throw std::runtime_error("超出定义域");
            if (result == nullptr)
                result = dynamic_cast<Set *>(args[i].get())->copyToSetPtr();
            else
                result = result->product(dynamic_cast<Set *>(args[i].get())->copyToSetPtr());
        }
        return result;
    }
};
typedef MArgFuncMapping<SetMapping, Set, ProductSetFunction, SetSymbol> ProductSetMapping;

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