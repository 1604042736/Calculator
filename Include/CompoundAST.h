#pragma once

#include "AST.h"

class CompoundAST : public AST
{
public:
    CompoundAST(std::vector<astptr_t> children, Context context) : AST(children, context) {}

    virtual objptr_t exec(Runtime *);
};