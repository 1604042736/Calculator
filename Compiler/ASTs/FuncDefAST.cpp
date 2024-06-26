#include <functional>
#include <map>

#include "FuncDefAST.h"
#include "Mapping.h"
#include "ExprMapping.h"
#include "SetMapping.h"
#include "Lambda.h"
#include "NameAST.h"
#include "Error.h"

objptr_t FuncDefAST::exec(Runtime *runtime)
{
    mappingptr_t mapping;
    if (runtime->flags.test(EXPR_FLAG))
        mapping = mappingptr_t(new ExprMapping(this->name));
    else if (runtime->flags.test(SET_FLAG))
        mapping = mappingptr_t(new SetMapping(this->name));
    else
        mapping = mappingptr_t(new Mapping(this->name));
    runtime->defName(this->name, mapping);

    funcargs_t args;
    for (size_t i = 0; i < this->args_name.size(); i++)
        args.push_back(NameAST(this->args_name[i], context).exec(runtime));

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
            mapping->sections.push_back(
                Lambda(lambdaargs,
                       expr_domain.args[0],
                       dynamic_cast<Set *>(expr_domain.args[1].get())->simplify()));
        }
        else
            mapping->sections.push_back(Lambda(lambdaargs, expr_domain.args[0]));
    }
    return nullptr;
}