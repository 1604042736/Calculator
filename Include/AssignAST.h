#pragma once

#include "AST.h"

class AssignAST : public AST
{
public:
    AssignAST(std::string name, astptr_t expr, Context context) : name(name), AST({expr}, context) {}

    virtual std::string getInfo()
    {
        return AST::getInfo() + " " + this->name;
    }

    virtual objptr_t exec(Runtime *);

    std::string name;
};