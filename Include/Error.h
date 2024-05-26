#pragma once

#include "Context.h"

/*错误*/
class Error
{
public:
    Error(std::string message, Context context) : message(message), context(context) {}

    std::string message;
    Context context;
};

class NameError : public Error
{
public:
    NameError(std::string name, Context context) : Error("'" + name + "' 未定义", context) {}
};

class SyntaxError : public Error
{
public:
    SyntaxError(std::string message, Context context) : Error(message, context) {}
};

class TooFewTokenError : public SyntaxError
{
public:
    TooFewTokenError(Context context) : SyntaxError("Token太少", context) {}
};