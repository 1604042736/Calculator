#pragma once

#include "Set.h"
#include "Expression.h"

class EnumSet;
/*区间*/
class Interval : public Set
{
public:
    Interval(exprptr_t start, exprptr_t end, bool left_open, bool right_open)
        : start(start), end(end), left_open(left_open), right_open(right_open) {}

    virtual Object *copyThis() { return new Interval(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual setptr_t pow(EnumSet);
    virtual Interval pow(Interval);
    virtual setptr_t pow(setptr_t);

    virtual setptr_t operator|(Interval);
    virtual setptr_t operator|(setptr_t);

    virtual setptr_t operator&(Interval);
    virtual setptr_t operator&(setptr_t);

    virtual boolptr_t includes(Interval);
    virtual boolptr_t includes(setptr_t);

    virtual boolptr_t operator==(Interval b) { return start == b.start && end == b.end && to_boolean(left_open == b.left_open) && to_boolean(right_open == b.right_open); }
    virtual boolptr_t operator==(setptr_t);

    bool left_open;  // 是否为左开
    bool right_open; // 是否为右开
    exprptr_t start; // 左边界
    exprptr_t end;   // 右边界
};