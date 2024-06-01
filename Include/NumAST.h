#pragma once

#include "AST.h"

class NumAST : public AST
{
public:
    NumAST(std::string num, Context context) : num(num), AST(context) {}

    virtual std::string getInfo() { return AST::getInfo() + " " + this->num; }

    std::string num;
};