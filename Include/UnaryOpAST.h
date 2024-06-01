#pragma once

#include "AST.h"

class UnaryOpAST : public AST
{
public:
    UnaryOpAST(std::string op, astptr_t expr, Context context) : op(op), AST({expr}, context) {}

    virtual objptr_t exec(Runtime *);

    std::string op;
};