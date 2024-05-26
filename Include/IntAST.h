#pragma once

#include "NumAST.h"

class IntAST : public NumAST
{
public:
    IntAST(std::string num, Context context) : NumAST(num, context) {}

    virtual objptr_t exec(Runtime *);
};