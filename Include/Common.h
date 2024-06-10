#pragma once

#include <memory>

#include "Object.h"
#include "Mapping.h"
#include "Runtime.h"
#include "ExprMapping.h"

template <typename T, typename T2>
bool inline isinstance(std::shared_ptr<T2> a) { return dynamic_cast<T *>(a.get()) != nullptr; }

template <typename T, typename T2>
bool inline isinstance(T2 *a) { return dynamic_cast<T *>(a) != nullptr; }

objptr_t exec(std::string, std::string, Runtime *runtime = nullptr, bool verbose = false);
objptr_t exec(std::string, Runtime *runtime = nullptr, bool verbose = false, bool run_only = false);

void shell(Runtime *, bool verbose = false);
void shell(bool verbose = false);

class SimplifyMapping : public ExprMapping
{
public:
    SimplifyMapping() : ExprMapping("simplify") {}

    virtual objptr_t operator()(funcargs_t);
};

class EvalMapping : public ExprMapping
{
public:
    EvalMapping() : ExprMapping("eval", RealSet().product(IntegerSet()), setptr_t(new RationalSet())) {}

    virtual exprptr_t operator()(exprptr_t b, Integer c) { return b->eval(c); }
    virtual objptr_t operator()(funcargs_t);
};

class PrintMapping : public Mapping
{
public:
    PrintMapping() : Mapping("print", setptr_t(new UniversalSet()), setptr_t(new EmptySet())) {}

    virtual objptr_t operator()(funcargs_t);
};

class DiffMapping : public ExprMapping
{
public:
    DiffMapping() : ExprMapping("diff") {}

    virtual objptr_t operator()(funcargs_t);
};

class FactorintMapping : public ExprMapping
{
public:
    FactorintMapping();

    virtual objptr_t operator()(funcargs_t);
};

class ProductSetMapping : public Mapping
{
public:
    ProductSetMapping() : Mapping("productset") {}

    virtual objptr_t operator()(funcargs_t);
};