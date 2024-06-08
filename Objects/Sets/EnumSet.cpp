#include <iostream>

#include "EnumSet.h"
#include "True.h"
#include "EmptySet.h"
#include "Common.h"
#include "Tuple.h"

std::string EnumSet::toString()
{
    std::string result = "{";
    for (size_t i = 0; i < this->elements.size(); i++)
    {
        result += this->elements[i]->toString();
        if (i < this->elements.size() - 1)
            result += ", ";
    }
    result += "}";
    return result;
}

prettystring_t EnumSet::toPrettyString()
{
    prettystring_t result;
    for (size_t i = 0; i < this->elements.size(); i++)
    {
        prettystring_t element_str = this->elements[i]->toPrettyString();
        if (result.size() == 0)
            result = element_str;
        else
        {
            if (element_str.size() > result.size())
            {
                // 进行扩充
                size_t d = (element_str.size() - result.size());
                for (size_t j = 0; j < d; j++)
                    result.insert(result.begin(), std::string(result[0].size(), ' '));
            }
            // element_str的最后一行与result的最后一行对齐
            size_t y = result.size() - element_str.size();
            for (size_t j = 0; j < element_str.size(); j++, y++)
                result[y] += element_str[j];
        }
        if (i < this->elements.size() - 1)
            result.back() += ", ";
        result = normalize(result);
    }
    for (size_t i = 0; i < result.size(); i++)
        result[i] = "{ " + result[i] + " }";
    return normalize(result);
}

boolptr_t EnumSet::contains(objptr_t element)
{
    for (size_t i = 0; i < this->elements.size(); i++)
    {
        if (isinstance<True>(this->elements[i] == element))
            return to_boolean(true);
    }
    return Set::contains(element);
}

EnumSet EnumSet::operator|(EnumSet b)
{
    elements_t elements(this->elements);
    for (size_t i = 0; i < b.elements.size(); i++)
    {
        bool flag = false;
        for (size_t j = 0; j < elements.size(); j++) // 查找是否有相同的
        {
            if (isinstance<True>(b.elements[i] == elements[j]))
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            elements.push_back(b.elements[i]);
    }
    return EnumSet(elements);
}

setptr_t EnumSet::operator|(setptr_t b)
{
    if (isinstance<EnumSet>(b))
        return setptr_t(new EnumSet(*this | *(EnumSet *)b.get()));
    return Set::operator|(b);
}

setptr_t EnumSet::operator&(EnumSet b)
{
    elements_t elements;
    for (size_t i = 0; i < b.elements.size(); i++)
    {
        bool flag = false;
        for (size_t j = 0; j < this->elements.size(); j++) // 查找是否有相同的
        {
            if (b.elements[i] == this->elements[j])
            {
                flag = true;
                break;
            }
        }
        if (flag)
            elements.push_back(b.elements[i]);
    }
    if (elements.size() == 0)
        return setptr_t(new EmptySet());
    return setptr_t(new EnumSet(elements));
}

setptr_t EnumSet::operator&(setptr_t b)
{
    if (isinstance<EnumSet>(b))
        return *this & *(EnumSet *)b.get();
    return Set::operator|(b);
}

setptr_t EnumSet::product(EnumSet b)
{
    elements_t elements;
    for (size_t i = 0; i < this->elements.size(); i++)
        for (size_t j = 0; j < b.elements.size(); j++)
            elements.push_back(objptr_t(new Tuple({this->elements[i], b.elements[j]})));
    if (elements.size() == 0)
        return setptr_t(new EmptySet());
    return setptr_t(new EnumSet(elements));
}

setptr_t EnumSet::product(setptr_t b)
{
    if (isinstance<EnumSet>(b))
        return this->product(*(EnumSet *)b.get());
    return Set::product(b);
}

setptr_t EnumSet::productpow(Integer n)
{
    Integer base(this->elements.size());
    Integer end = dynamic_cast<Integer &>(*(base.pow(n)).get());
    elements_t elements;
    for (Integer i = 0; i < end; i = i + 1)
    {
        tupleargs_t args;
        Integer t = i;
        for (Integer j = 0; j < n; j = j + 1)
        {
            args.push_back(this->elements[(t % base).toUInt64()]);
            t = floordiv(t, base);
        }
        elements.push_back(objptr_t(new Tuple(args)));
    }
    if (elements.size() == 0)
        return setptr_t(new EmptySet());
    return setptr_t(new EnumSet(elements));
}