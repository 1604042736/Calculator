#pragma once

#include "AST.h"

class ImportAST : public AST
{
public:
    ImportAST(std::string name, Context context) : name(name), AST(context) {}

    virtual std::string getInfo() { return AST::getInfo() + " " + this->name; }

    virtual objptr_t exec(Runtime *);

    std::string name;
};