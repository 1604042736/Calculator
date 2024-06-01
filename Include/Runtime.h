#pragma once

#include <vector>
#include <map>
#include <bitset>

#include "Object.h"
#include "Expression.h"
#include "Mapping.h"
#include "Integer.h"
#include "EmptySet.h"

#define EXPR_FLAG 0
#define BOOL_FLAG 1

typedef std::bitset<16> flags_t;

/*运行时*/
class Runtime
{
public:
    Runtime();

    objptr_t findName(std::string);
    void defName(std::string, objptr_t);

    std::vector<std::map<std::string, objptr_t>> scopes; // 作用域, 上层作用域放在大索引处
    flags_t flags;                                       // 标志位
};

class SimplifyMapping : public Mapping
{
public:
    SimplifyMapping() : Mapping("simplify", setptr_t(new RealSet()), setptr_t(new RealSet())) {}

    virtual exprptr_t operator()(exprptr_t b) { return b->simplify(); }
    virtual objptr_t operator()(funcargs_t);
};

class EvalMapping : public Mapping
{
public:
    EvalMapping() : Mapping("eval", RealSet() * IntegerSet(), setptr_t(new RationalSet())) {}

    virtual exprptr_t operator()(exprptr_t b, Integer c) { return b->eval(c); }
    virtual objptr_t operator()(funcargs_t);
};

class PrintMapping : public Mapping
{
public:
    PrintMapping() : Mapping("print", setptr_t(new UniversalSet()), setptr_t(new EmptySet())) {}

    virtual objptr_t operator()(funcargs_t);
};