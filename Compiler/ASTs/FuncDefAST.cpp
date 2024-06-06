#include <functional>
#include <map>

#include "FuncDefAST.h"
#include "Mapping.h"
#include "ExprMapping.h"
#include "DefinedFunction.h"
#include "ExprDefFunction.h"
#include "Lambda.h"
#include "NameAST.h"
#include "Common.h"
#include "Error.h"

std::map<std::string, deffuncsec_t> funcsec_map;

objptr_t FuncDefAST::exec(Runtime *runtime)
{
    std::function<objptr_t(std::string)> func_builder;
    if (this->args_name.size() == 0 || this->body.size() == 0)
    {
        if (runtime->flags.test(EXPR_FLAG))
        {
            func_builder = [](std::string name)
            { return objptr_t(new ExprMapping(name)); };
        }
        else
            func_builder = [](std::string name)
            { return objptr_t(new Mapping(name)); };
        runtime->defName(this->name, func_builder(this->name));
        return nullptr;
    }

    funcargs_t args;
    for (size_t i = 0; i < this->args_name.size(); i++)
        args.push_back(NameAST(this->args_name[i], context).exec(runtime));

    funcsec_map[this->name] = deffuncsec_t();
    if (runtime->flags.test(EXPR_FLAG))
    {
        func_builder = [&](std::string name)
        { return objptr_t(new ExprDefFunction(name, args, &funcsec_map[this->name])); };
    }
    else
        func_builder = [&](std::string name)
        { return objptr_t(new DefinedFunction(name, args, &funcsec_map[this->name])); };
    runtime->defName(this->name, func_builder(this->name));

    lambdaargs_t lambdaargs;
    for (size_t i = 0; i < args.size(); i++)
    {
        if (!isinstance<Symbol>(args[i]))
            throw Error("参数必须是符号", context);
        lambdaargs.push_back(symptr_t(dynamic_cast<Symbol *>(args[i]->copyThis())));
    }
    for (size_t i = 0; i < this->body.size(); i++)
    {
        Tuple expr_domain = dynamic_cast<Tuple &>(*this->body[i]->exec(runtime).get());
        if (expr_domain.args.size() > 1)
        {
            if (!isinstance<Set>(expr_domain.args[1]))
                throw Error("定义域不是集合", context);
            funcsec_map[this->name].push_back(
                Lambda(lambdaargs,
                       expr_domain.args[0],
                       setptr_t(dynamic_cast<Set *>(expr_domain.args[1]->copyThis()))));
        }
        else
            funcsec_map[this->name].push_back(Lambda(lambdaargs, expr_domain.args[0]));
    }
    return nullptr;
}