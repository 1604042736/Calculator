#pragma once

#include "AST.h"

/*修饰*/
class ModifyAST : public AST
{
public:
    ModifyAST(std::string mode, astptr_t target, Context context)
        : mode(mode), target(target), AST({target}, context) {}

    virtual std::string getInfo() { return AST::getInfo() + " " + this->mode; }

    virtual objptr_t exec(Runtime *);

    std::string mode;
    astptr_t target;
};