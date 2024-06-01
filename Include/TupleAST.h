#pragma once

#include "AST.h"

class TupleAST : public AST
{
public:
    TupleAST(std::vector<astptr_t> args, Context context) : AST(args, context) {}

    virtual objptr_t exec(Runtime *);
};