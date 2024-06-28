#include <string>
#include <sstream>
#include <iostream>

#include "Object.h"
#include "Boolean.h"
#include "True.h"
#include "Expression.h"
#include "UniversalSet.h"
#include "Function.h"
#include "Equality.h"
#include "Unequality.h"
#include "LessThan.h"
#include "StrictLessThan.h"
#include "StrictGreaterThan.h"
#include "GreaterThan.h"

/*转换成字符串*/
std::string Object::toString()
{
    std::stringstream ss;
    ss << '[' << typeid(*this).name() << '(' << this << ')' << ']';
    return ss.str();
}

objptr_t Object::operator+(objptr_t b)
{
    throw std::runtime_error("Unsupported operator+ for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
objptr_t operator+(objptr_t a, objptr_t b) { return a->operator+(b); }

objptr_t Object::operator-(objptr_t b)
{
    throw std::runtime_error("Unsupported operator- for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
objptr_t operator-(objptr_t a, objptr_t b) { return a->operator-(b); }

objptr_t Object::operator*(objptr_t b)
{
    throw std::runtime_error("Unsupported operator* for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
objptr_t operator*(objptr_t a, objptr_t b) { return a->operator*(b); }

objptr_t Object::operator/(objptr_t b)
{
    throw std::runtime_error("Unsupported operator/ for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
objptr_t operator/(objptr_t a, objptr_t b) { return a->operator/(b); }

objptr_t Object::pow(objptr_t b)
{
    throw std::runtime_error("Unsupported pow for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}

boolptr_t Object::operator&&(objptr_t b)
{
    throw std::runtime_error("Unsupported operator&& for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
boolptr_t operator&&(objptr_t a, objptr_t b) { return a->operator&&(b); }

boolptr_t Object::operator||(objptr_t b)
{
    throw std::runtime_error("Unsupported operator|| for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
boolptr_t operator||(objptr_t a, objptr_t b) { return a->operator||(b); }

boolptr_t Object::operator!()
{
    throw std::runtime_error("Unsupported operator! for " + std::string(typeid(*this).name()));
}

boolptr_t Object::operator==(objptr_t b) { return boolptr_t(new Equality(to_ptr<Object>(this), b)); }
boolptr_t operator==(objptr_t a, objptr_t b) { return a->operator==(b); }

boolptr_t Object::operator!=(objptr_t b) { return !this->operator==(b); }
boolptr_t operator!=(objptr_t a, objptr_t b) { return a->operator!=(b); }

boolptr_t Object::operator>(objptr_t b)
{
    return boolptr_t(new StrictGreaterThan(to_ptr<Object>(this), b));
}
boolptr_t operator>(objptr_t a, objptr_t b) { return a->operator>(b); }

boolptr_t Object::operator>=(objptr_t b) { return *this == b || *this > b; }
boolptr_t operator>=(objptr_t a, objptr_t b) { return a->operator>=(b); }

boolptr_t Object::operator<(objptr_t b)
{
    return boolptr_t(new StrictLessThan(to_ptr<Object>(this), b));
}
boolptr_t operator<(objptr_t a, objptr_t b) { return a->operator<(b); }

boolptr_t Object::operator<=(objptr_t b) { return *this == b || *this < b; }
boolptr_t operator<=(objptr_t a, objptr_t b) { return a->operator<=(b); }

objptr_t Object::operator&(objptr_t b)
{
    throw std::runtime_error("Unsupported operator& for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
objptr_t operator&(objptr_t a, objptr_t b) { return a->operator&(b); }

objptr_t Object::operator|(objptr_t b)
{
    throw std::runtime_error("Unsupported operator& for " + std::string(typeid(*this).name()) + " and " + std::string(typeid(*b.get()).name()));
}
objptr_t operator|(objptr_t a, objptr_t b) { return a->operator|(b); }

objptr_t Object::operator()(std::vector<objptr_t>)
{
    throw std::runtime_error("Unsupported operator() for " + std::string(typeid(*this).name()));
}

objptr_t Object::replace(objptr_t old, objptr_t _new)
{
    if (isinstance<True>(this->operator==(old)))
        return _new;
    return objptr_t(this->copyThis());
}

setptr_t Object::belongto()
{
    return setptr_t(new UniversalSet());
}

/*
规范化pretty string
保证每行字符串长度相同
*/
prettystring_t normalize(prettystring_t pstr)
{
    // 填充空格
    size_t max_len = 0;
    for (size_t i = 0; i < pstr.size(); i++)
        max_len = std::max(pstr[i].size(), max_len);
    for (size_t i = 0; i < pstr.size(); i++)
        pstr[i] = pstr[i] + std::string(max_len - pstr[i].size(), ' ');
    // 删除空行
    return pstr;
    for (size_t i = 0; i < pstr.size(); i++)
    {
        bool flag = true;
        for (size_t j = 0; j < pstr[i].size(); j++)
        {
            if (pstr[i][j] != ' ')
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            pstr.erase(pstr.begin() + i);
            i--;
        }
    }

    return pstr;
}

void print(prettystring_t pstr)
{
    for (size_t i = 0; i < pstr.size(); i++)
        std::cout << pstr[i] << std::endl;
}

/*
化简
对于无法化简的对象不做任何操作
*/
objptr_t _simplify(objptr_t a)
{
    if (isinstance<Expression>(a))
        return dynamic_cast<Expression *>(a.get())->simplify();
    else if (isinstance<Boolean>(a))
        return dynamic_cast<Boolean *>(a.get())->simplify();
    else if (isinstance<Set>(a))
        return dynamic_cast<Set *>(a.get())->simplify();
    else if (isinstance<Function>(a))
        return dynamic_cast<Function *>(a.get())->_simplify_();
    return a;
}

objptr_t simplify(objptr_t a)
{
    objptr_t result = _simplify(a);
    objptr_t pre;
    do
    {
        pre = result;
        result = _simplify(result);
        if (result == nullptr || pre == nullptr)
            break;
    } while (isinstance<True>(result != pre));

    return result;
}