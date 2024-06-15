#include "Interval.h"

#include "EnumSet.h"
#include "EmptySet.h"
#include "Integer.h"

std::string Interval::toString()
{
    std::string result = "";
    if (this->left_open)
        result += "(";
    else
        result += "[";
    result += this->start->toString();
    result += ", ";
    result += this->end->toString();
    if (this->right_open)
        result += ")";
    else
        result += "]";
    return result;
}

prettystring_t Interval::toPrettyString()
{
    prettystring_t result = this->start->toPrettyString();
    result.back() += ", ";
    result = normalize(result);

    prettystring_t end_str = this->end->toPrettyString();
    size_t d = end_str.size() - result.size();
    for (size_t i = 0; i < d; i++)
        result.insert(result.begin(), std::string(result[0].size(), ' '));
    for (size_t i = 0; i < end_str.size(); i++)
        result[i] += end_str[i];

    std::string l, r;
    if (this->left_open)
        l = "(";
    else
        l = "[";
    if (this->right_open)
        r = ")";
    else
        r = "]";

    for (size_t i = 0; i < result.size(); i++)
        result[i] = l + result[i] + r;

    return normalize(result);
}

setptr_t Interval::pow(EnumSet b)
{
    setptr_t result = nullptr;
    for (size_t i = 0; i < b.elements.size(); i++)
    {
        objptr_t s = start->pow(b.elements[i]);
        objptr_t e = end->pow(b.elements[i]);
        setptr_t t;
        if (isinstance<Integer>(b.elements[i]))
        {
            Integer n = *dynamic_cast<Integer *>(b.elements[i].get());
            if (n > 0 && n % 2 == 0)
            {
                if (isinstance<True>(start <= Integer(0) && end >= Integer(0)))
                {
                    if (isinstance<True>(s >= e))
                    {
                        e = s;
                        s = exprptr_t(new Integer(0));
                    }
                    else if (isinstance<True>(s < e))
                        s = exprptr_t(new Integer(0));
                }
                else if (isinstance<True>(start <= Integer(0) && end <= Integer(0)))
                    std::swap(s, e);
            }
        }
        t = setptr_t(new Interval(dynamic_cast<Expression *>(s.get())->eval(2), // copyToExprPtr(),
                                  dynamic_cast<Expression *>(e.get())->eval(3), // copyToExprPtr(),
                                  left_open,
                                  right_open));
        if (result == nullptr)
            result = t;
        else
            result = result->operator|(t);
    }
    return result;
}

Interval Interval::pow(Interval b)
{
    // 其实这样是不准确的
    return Interval(start->pow(b.start), end->pow(b.end), left_open || b.left_open, right_open || b.right_open);
}

setptr_t Interval::pow(setptr_t b)
{
    if (isinstance<Interval>(b))
        return this->pow(*dynamic_cast<Interval *>(b.get())).copyToSetPtr();
    else if (isinstance<EnumSet>(b))
        return this->pow(*dynamic_cast<EnumSet *>(b.get()));
    return Set::pow(b);
}

setptr_t Interval::operator|(Interval b)
{
    boolptr_t t = this->includes(b);
    if (isinstance<True>(t))
        return this->copyToSetPtr();

    t = b.includes(*this);
    if (isinstance<True>(t))
        return b.copyToSetPtr();

    if (!right_open || !b.left_open)
    {
        if (isinstance<True>(end >= b.start))
            return setptr_t(new Interval(start, b.end, left_open, b.right_open));
    }
    if (!left_open || !b.right_open)
    {
        if (isinstance<True>(b.end >= start))
            return setptr_t(new Interval(b.start, end, b.left_open, right_open));
    }
    return Set::operator|(b.copyToSetPtr());
}

setptr_t Interval::operator|(setptr_t b)
{
    if (isinstance<Interval>(b))
        return this->operator|(*dynamic_cast<Interval *>(b.get()));
    return Set::operator|(b);
}

setptr_t Interval::operator&(Interval b)
{
    boolptr_t t = this->includes(b);
    if (isinstance<True>(t))
        return b.copyToSetPtr();

    t = b.includes(*this);
    if (isinstance<True>(t))
        return this->copyToSetPtr();

    t = (left_open ? start >= b.end : start > b.end);
    t = t || (right_open ? end <= b.start : end < b.start);
    if (isinstance<True>(t))
        return setptr_t(new EmptySet());

    if (!right_open || !b.left_open)
    {
        if (isinstance<True>(end >= b.start))
            return setptr_t(new Interval(b.start, end, b.left_open, right_open));
    }
    if (!left_open || !b.right_open)
    {
        if (isinstance<True>(b.end >= start))
            return setptr_t(new Interval(start, b.end, left_open, b.right_open));
    }
    return Set::operator&(b.copyToSetPtr());
}

setptr_t Interval::operator&(setptr_t b)
{
    if (isinstance<Interval>(b))
        return this->operator&(*dynamic_cast<Interval *>(b.get()));
    return Set::operator&(b);
}

boolptr_t Interval::includes(Interval b)
{
    boolptr_t result = (left_open ? start < b.start : start <= b.start);
    result = result && (right_open ? end > b.end : end >= b.end);
    return result;
}

boolptr_t Interval::includes(setptr_t b)
{
    if (isinstance<Interval>(b))
        return this->includes(*dynamic_cast<Interval *>(b.get()));
    return Set::includes(b);
}

boolptr_t Interval::operator==(setptr_t b)
{
    if (isinstance<Interval>(b))
        return this->operator==(*dynamic_cast<Interval *>(b.get()));
    return Set::operator==(b);
}