#pragma once

#include "AST.h"

/*表达式修饰*/
class ExprModAST : public AST
{
public:
    ExprModAST(astptr_t target, Context context) : target(target), AST({target}, context) {}

    virtual objptr_t exec(Runtime *);

    astptr_t target;
};