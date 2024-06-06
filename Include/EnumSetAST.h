#pragma once

#include "AST.h"

class EnumSetAST : public AST
{
public:
    EnumSetAST(std::vector<astptr_t> elements, Context context) : AST(elements, context) {}

    virtual objptr_t exec(Runtime *);
};