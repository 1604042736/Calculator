#pragma once

#include "AST.h"

class ScopeAST : public AST
{
public:
    ScopeAST(std::vector<std::string> names, astptr_t stmt, Context context)
        : names(names), AST({stmt}, context) {}

    virtual std::string getInfo()
    {
        std::string result = AST::getInfo() + " ";
        for (size_t i = 0; i < names.size(); i++)
            result += names[i] + " ";
        return result;
    }

    virtual objptr_t exec(Runtime *);

    std::vector<std::string> names;
};