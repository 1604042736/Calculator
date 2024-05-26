#pragma once

#include "AST.h"

/*二元运算符语法树*/
class BinOpAST : public AST
{
public:
    BinOpAST(std::string op, astptr_t l, astptr_t r, Context context) : op(op), AST({l, r}, context) {}

    virtual std::string getInfo() { return AST::getInfo() + " " + this->op; }

    virtual objptr_t exec(Runtime *);

    std::string op;
};