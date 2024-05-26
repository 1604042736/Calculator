#pragma once

#include "AST.h"

class CallAST : public AST
{
public:
    CallAST(astptr_t func, std::vector<astptr_t> args, Context context)
        : func(func), args(args), AST(context)
    {
        this->children.push_back(func);
        for (size_t i = 0; i < args.size(); i++)
            this->children.push_back(args[i]);
    }

    astptr_t func;
    std::vector<astptr_t> args;

    virtual objptr_t exec(Runtime *);
};