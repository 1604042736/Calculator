#pragma once

#include <memory>

#include "Object.h"
#include "Mapping.h"
#include "Runtime.h"

template <typename T, typename T2>
bool inline isinstance(std::shared_ptr<T2> a) { return dynamic_cast<T *>(a.get()) != nullptr; }

template <typename T, typename T2>
bool inline isinstance(T2 *a) { return dynamic_cast<T *>(a) != nullptr; }

objptr_t exec(std::string, std::string, Runtime *, bool verbose = false);
objptr_t exec(std::string, std::string, bool verbose = false);

void shell(Runtime *, bool verbose = false);
void shell(bool verbose = false);

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

class DiffMapping : public Mapping
{
public:
    DiffMapping() : Mapping("diff") {}

    virtual objptr_t operator()(funcargs_t);
};

class FactorintMapping : public Mapping
{
public:
    FactorintMapping();

    virtual objptr_t operator()(funcargs_t);
};