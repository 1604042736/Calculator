#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Object.h"
#include "Context.h"
#include "Runtime.h"

class AST;
typedef std::shared_ptr<AST> astptr_t;

/*语法树*/
class AST
{
public:
    AST(std::vector<astptr_t> children, Context context) : children(children), context(context) {}
    AST(Context context) : context(context) {}

    virtual std::string getInfo() { return std::string(typeid(*this).name()) + "[" + this->context.filename + "," + std::to_string(context.line) + "," + std::to_string(context.column) + "]"; }
    virtual void print(size_t level = 0, std::string indent = "    ");

    /*运行*/
    virtual objptr_t exec(Runtime *) = 0;

    std::vector<astptr_t> children;
    Context context;
};