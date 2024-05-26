#pragma once

#include "Set.h"
#include "Boolean.h"
#include "Function.h"
#include "Lambda.h"

/*用描述法表示的集合*/
class DescribeSet : public Set
{
public:
    DescribeSet(Lambda func, boolptr_t condition) : func(func), condition(condition) {}

    virtual Object *copyThis() { return new DescribeSet(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    Lambda func;         // 将符合条件的元素映射为该集合里的元素
    boolptr_t condition; // 条件
};