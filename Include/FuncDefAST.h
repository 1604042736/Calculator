#pragma once

#include "AST.h"
#include "Tuple.h"

class FuncBodyAST : public AST
{
public:
    FuncBodyAST(astptr_t expr, astptr_t domain, Context context) : AST({expr}, context)
    {
        if (domain != nullptr)
            this->children.push_back(domain);
    }

    virtual objptr_t exec(Runtime *runtime)
    {
        objptr_t expr = this->children[0]->exec(runtime);
        if (this->children.size() > 1)
            return objptr_t(new Tuple({expr, this->children[1]->exec(runtime)}));
        return objptr_t(new Tuple({expr}));
    }
};

typedef std::shared_ptr<FuncBodyAST> funcbodyastptr_t;

class FuncDefAST : public AST
{
public:
    FuncDefAST(std::string name,
               std::vector<std::string> args_name,
               std::vector<funcbodyastptr_t> body,
               Context context)
        : name(name), args_name(args_name), body(body), AST(context)
    {
        for (size_t i = 0; i < body.size(); i++)
            this->children.push_back(body[i]);
    }

    virtual std::string getInfo()
    {
        std::string result = AST::getInfo() + " " + name + " ";
        for (size_t i = 0; i < args_name.size(); i++)
            result += args_name[i] + " ";
        return result;
    }

    virtual objptr_t exec(Runtime *);

    std::string name;
    std::vector<std::string> args_name;
    std::vector<funcbodyastptr_t> body;
};