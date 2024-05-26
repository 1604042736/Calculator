#pragma once

#include "Set.h"
#include "Expression.h"

/*区间*/
class Interval : public Set
{
public:
    Interval(exprptr_t start, exprptr_t end, bool left_open, bool right_open)
        : start(start), end(end), left_open(left_open), right_open(right_open) {}

    virtual Object *copyThis() { return new Interval(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    bool left_open;  // 是否为左开
    bool right_open; // 是否为右开
    exprptr_t start; // 左边界
    exprptr_t end;   // 右边界
};