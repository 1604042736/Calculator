#include <stdexcept>
#include <iostream>

#include "Runtime.h"
#include "Sqrt.h"
#include "Common.h"
#include "PI.h"
#include "E.h"
#include "Log.h"
#include "Infinity.h"

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

objptr_t SimplifyMapping::operator()(funcargs_t args)
{
    if (args.size() > 1 || !isinstance<Expression>(args[0]))
        throw std::runtime_error("超出定义域");
    return this->operator()(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr());
}

objptr_t EvalMapping::operator()(funcargs_t args)
{
    if (args.size() != 2 || !isinstance<Expression>(args[0]) || !isinstance<Integer>(args[1]))
        throw std::runtime_error("超出定义域");
    return this->operator()(dynamic_cast<Expression *>(args[0].get())->copyToExprPtr(),
                            *dynamic_cast<Integer *>(args[1].get()));
}

objptr_t PrintMapping::operator()(funcargs_t args)
{
    for (size_t i = 0; i < args.size(); i++)
        print(args[i]->toPrettyString());
    return nullptr;
}