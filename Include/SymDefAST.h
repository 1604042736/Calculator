#pragma once

#include "AST.h"

/*符号定义*/
class SymDefAST : public AST
{
public:
    SymDefAST(std::vector<std::string> names, Context context) : names(names), AST(context) {}

    virtual objptr_t exec(Runtime *);

    virtual std::string getInfo()
    {
        std::string result = AST::getInfo() + " " + this->names[0];
        for (size_t i = 1; i < this->names.size(); i++)
            result += ", " + this->names[i];
        return result;
    }

    std::vector<std::string> names;
};