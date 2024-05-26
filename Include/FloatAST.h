#pragma once

#include "NumAST.h"

class FloatAST : public NumAST
{
public:
    FloatAST(std::string num, Context context) : NumAST(num, context) {}

    virtual objptr_t exec(Runtime *);
};