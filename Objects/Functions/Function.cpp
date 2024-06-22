#include "Function.h"
#include "Mapping.h"
#include "Tuple.h"
#include "True.h"

Function::Function(mappingptr_t mapping, funcargs_t args) : mapping(mapping), args(args)
{
    this->name = mapping->name;
}

boolptr_t Function::operator==(objptr_t b)
{
    if (isinstance<Function>(b))
    {
        Function *f = dynamic_cast<Function *>(b.get());
        if (this->args.size() != f->args.size())
            return to_boolean(false);
        boolptr_t result = to_boolean(this->name == f->name);
        for (size_t i = 0; i < args.size(); i++)
            result = result->operator&&(args[i] == f->args[i]);
        return result;
    }
    return Object::operator==(b);
}

objptr_t Function::replace(objptr_t old, objptr_t _new)
{
    if (isinstance<True>(this->operator==(old)))
        return _new;

    funcargs_t n_args;
    for (size_t i = 0; i < this->args.size(); i++)
        n_args.push_back(this->args[i]->replace(old, _new));
    if (this->mapping != nullptr)
        return simplify(this->mapping->operator()(n_args));
    funcptr_t f(dynamic_cast<Function *>(copyThis()));
    f->setArgs(n_args);
    return f;
}

boolptr_t in_domain(setptr_t domain, funcargs_t args)
{
    if (args.size() == 1)
        return domain->contains(args[0]);
    objptr_t t(new Tuple({args[0], args[1]}));
    //(a,b,c,...) => (((a,b),c),...)
    for (size_t i = 2; i < args.size(); i++)
        t = objptr_t(new Tuple({t, args[i]}));
    return domain->contains(t);
}