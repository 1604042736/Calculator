#include <iostream>

#include "Expression.h"
#include "Add.h"
#include "Mul.h"
#include "Integer.h"
#include "Pow.h"
#include "Sqrt.h"
#include "Integer.h"
#include "Float.h"
#include "Reciprocal.h"
#include "Opposite.h"
#include "Abs.h"
#include "GreaterThan.h"
#include "StrictGreaterThan.h"
#include "LessThan.h"
#include "StrictLessThan.h"

#include "Derivative.h"
#include "Equality.h"
#include "ExprSymbol.h"

exprptr_t Expression::operator*(exprptr_t b)
{
    static std::vector<std::tuple<exprptr_t, exprptr_t>> on_cal; // 正在计算的数据

    exprptr_t self = this->copyToExprPtr();

    if (isinstance<True>(*this == Integer(0)) || isinstance<True>(b == Integer(0)))
        return exprptr_t(new Integer(0));
    else if (isinstance<True>(*this == Integer(1)))
        return b;
    else if (isinstance<True>(b == Integer(1)))
        return self;
    exprptr_t ret(new Mul({self, b}));
    for (auto t : on_cal)
    {
        // 防止递归调用
        if (!isinstance<False>(std::get<0>(t) == self) && !isinstance<False>(std::get<1>(t) == b))
            return ret;
        if (!isinstance<False>(std::get<1>(t) == self) && !isinstance<False>(std::get<0>(t) == b))
            return ret;
    }
    on_cal.push_back(std::make_tuple(self, b));

    exprptr_t t = b * self;
    if (!isinstance<True>(ret == t))
        ret = t;
    on_cal.pop_back();
    return ret;
}

exprptr_t Expression::operator+(exprptr_t b)
{
    static std::vector<std::tuple<exprptr_t, exprptr_t>> on_cal; // 正在计算的数据

    exprptr_t self = this->copyToExprPtr();

    if (isinstance<True>(this->operator==(Integer(0))))
        return b;
    else if (isinstance<True>(b == Integer(0)))
        return self;

    exprptr_t ret(new Add({self, b}));
    for (auto t : on_cal)
    {
        // 防止递归调用
        if (!isinstance<False>(std::get<0>(t) == self) && !isinstance<False>(std::get<1>(t) == b))
            return ret;
        if (!isinstance<False>(std::get<1>(t) == self) && !isinstance<False>(std::get<0>(t) == b))
            return ret;
    }
    on_cal.push_back(std::make_tuple(self, b));

    exprptr_t t = b + self;
    if (!isinstance<True>(ret == t))
        ret = t;

    on_cal.pop_back();
    return ret;
}

exprptr_t Expression::operator+(Expression &_1) { return this->operator+(_1.copyToExprPtr()); }
exprptr_t Expression::operator+(Expression &&_1) { return this->operator+(_1.copyToExprPtr()); }
exprptr_t operator+(exprptr_t _1, exprptr_t _2) { return _1->operator+(_2); }
exprptr_t operator+(exprptr_t _1, Expression &_2) { return _1->operator+(_2); }
exprptr_t operator+(exprptr_t _1, Expression &&_2) { return _1->operator+(_2); }
exprptr_t Expression::operator-(exprptr_t _1) { return this->operator+(_1->opposite()); }
exprptr_t Expression::operator-(Expression &_1) { return this->operator-(_1.copyToExprPtr()); }
exprptr_t Expression::operator-(Expression &&_1) { return this->operator-(_1.copyToExprPtr()); }
exprptr_t operator-(exprptr_t _1, exprptr_t _2) { return _1->operator-(_2); }
exprptr_t operator-(exprptr_t _1, Expression &_2) { return _1->operator-(_2); }
exprptr_t operator-(exprptr_t _1, Expression &&_2) { return _1->operator-(_2); }
exprptr_t Expression::operator*(Expression &_1) { return this->operator*(_1.copyToExprPtr()); }
exprptr_t Expression::operator*(Expression &&_1) { return this->operator*(_1.copyToExprPtr()); }
exprptr_t operator*(exprptr_t _1, exprptr_t _2) { return _1->operator*(_2); }
exprptr_t operator*(exprptr_t _1, Expression &_2) { return _1->operator*(_2); }
exprptr_t operator*(exprptr_t _1, Expression &&_2) { return _1->operator*(_2); }
exprptr_t Expression::operator/(exprptr_t _1) { return this->operator*(_1->reciprocal()); }
exprptr_t Expression::operator/(Expression &_1) { return this->operator/(_1.copyToExprPtr()); }
exprptr_t Expression::operator/(Expression &&_1) { return this->operator/(_1.copyToExprPtr()); }
exprptr_t operator/(exprptr_t _1, exprptr_t _2) { return _1->operator/(_2); }
exprptr_t operator/(exprptr_t _1, Expression &_2) { return _1->operator/(_2); }
exprptr_t operator/(exprptr_t _1, Expression &&_2) { return _1->operator/(_2); }
boolptr_t Expression::operator>(exprptr_t _1) { return boolptr_t(new StrictGreaterThan(this->copyToExprPtr(), _1)); }
boolptr_t Expression::operator>(Expression &_1) { return this->operator>(_1.copyToExprPtr()); }
boolptr_t Expression::operator>(Expression &&_1) { return this->operator>(_1.copyToExprPtr()); }
boolptr_t operator>(exprptr_t _1, exprptr_t _2) { return _1->operator>(_2); }
boolptr_t operator>(exprptr_t _1, Expression &_2) { return _1->operator>(_2); }
boolptr_t operator>(exprptr_t _1, Expression &&_2) { return _1->operator>(_2); }
boolptr_t Expression::operator>=(exprptr_t _1) { return (this->operator==(_1)) || (this->operator>(_1)); }
boolptr_t Expression::operator>=(Expression &_1) { return this->operator>=(_1.copyToExprPtr()); }
boolptr_t Expression::operator>=(Expression &&_1) { return this->operator>=(_1.copyToExprPtr()); }
boolptr_t operator>=(exprptr_t _1, exprptr_t _2) { return _1->operator>=(_2); }
boolptr_t operator>=(exprptr_t _1, Expression &_2) { return _1->operator>=(_2); }
boolptr_t operator>=(exprptr_t _1, Expression &&_2) { return _1->operator>=(_2); }
boolptr_t Expression::operator<(exprptr_t _1)
{
    return boolptr_t(new StrictLessThan(this->copyToExprPtr(), _1));
}
boolptr_t Expression::operator<(Expression &_1) { return this->operator<(_1.copyToExprPtr()); }
boolptr_t Expression::operator<(Expression &&_1) { return this->operator<(_1.copyToExprPtr()); }
boolptr_t operator<(exprptr_t _1, exprptr_t _2) { return _1->operator<(_2); }
boolptr_t operator<(exprptr_t _1, Expression &_2) { return _1->operator<(_2); }
boolptr_t operator<(exprptr_t _1, Expression &&_2) { return _1->operator<(_2); }
boolptr_t Expression::operator<=(exprptr_t _1) { return (this->operator==(_1)) || (this->operator<(_1)); }
boolptr_t Expression::operator<=(Expression &_1) { return this->operator<=(_1.copyToExprPtr()); }
boolptr_t Expression::operator<=(Expression &&_1) { return this->operator<=(_1.copyToExprPtr()); }
boolptr_t operator<=(exprptr_t _1, exprptr_t _2) { return _1->operator<=(_2); }
boolptr_t operator<=(exprptr_t _1, Expression &_2) { return _1->operator<=(_2); }
boolptr_t operator<=(exprptr_t _1, Expression &&_2) { return _1->operator<=(_2); }
boolptr_t Expression::operator==(exprptr_t _1) { return boolptr_t(new Equality(this->copyToExprPtr(), _1)); }
boolptr_t Expression::operator==(Expression &_1) { return this->operator==(_1.copyToExprPtr()); }
boolptr_t Expression::operator==(Expression &&_1) { return this->operator==(_1.copyToExprPtr()); }
boolptr_t operator==(exprptr_t _1, exprptr_t _2) { return _1->operator==(_2); }
boolptr_t operator==(exprptr_t _1, Expression &_2) { return _1->operator==(_2); }
boolptr_t operator==(exprptr_t _1, Expression &&_2) { return _1->operator==(_2); }
boolptr_t Expression::operator!=(exprptr_t _1) { return !(this->operator==(_1)); }
boolptr_t Expression::operator!=(Expression &_1) { return this->operator!=(_1.copyToExprPtr()); }
boolptr_t Expression::operator!=(Expression &&_1) { return this->operator!=(_1.copyToExprPtr()); }
boolptr_t operator!=(exprptr_t _1, exprptr_t _2) { return _1->operator!=(_2); }
boolptr_t operator!=(exprptr_t _1, Expression &_2) { return _1->operator!=(_2); }
boolptr_t operator!=(exprptr_t _1, Expression &&_2) { return _1->operator!=(_2); }
exprptr_t Expression::sqrt()
{
    static std::vector<std::tuple<exprptr_t>> on_cal; // 正在计算的数据

    exprptr_t self = this->copyToExprPtr();
    exprptr_t _0_5(new Float("0.5"));
    exprptr_t d(new Pow({self, _0_5}));

    for (auto t : on_cal)
    {
        // 防止递归调用
        if (!isinstance<False>(std::get<0>(t) == self))
            return exprptr_t(new Sqrt(self));
    }
    on_cal.push_back(std::make_tuple(self));

    exprptr_t t = this->pow(_0_5);
    if (isinstance<True>(t == d))
        return exprptr_t(new Sqrt(self));

    on_cal.pop_back();
    return t;
}
exprptr_t Expression::sqrt(Integer _1) { return this->pow(exprptr_t(new Float("0.5")), _1); }
exprptr_t sqrt(exprptr_t _1, Integer _2) { return _1->sqrt(_2); }
exprptr_t Expression::pow(exprptr_t b)
{
    static std::vector<std::tuple<exprptr_t, exprptr_t>> on_cal; // 正在计算的数据

    exprptr_t self = this->copyToExprPtr();

    if (isinstance<True>(b == Integer(1)))
        return self;

    exprptr_t ret(new Pow({self, b}));
    for (auto t : on_cal)
    {
        // 防止递归调用
        if (!isinstance<False>(std::get<0>(t) == self) && !isinstance<False>(std::get<1>(t) == b))
            return ret;
    }
    on_cal.push_back(std::make_tuple(self, b));

    exprptr_t t = b->rpow(self);
    if (!isinstance<True>(ret == t))
        ret = t;

    on_cal.pop_back();
    return ret;
}
exprptr_t Expression::pow(Expression &_1) { return this->pow(_1.copyToExprPtr()); }
exprptr_t Expression::pow(Expression &&_1) { return this->pow(_1.copyToExprPtr()); }
exprptr_t pow(exprptr_t _1, exprptr_t _2) { return _1->pow(_2); }
exprptr_t Expression::pow(exprptr_t _1, Integer _2) { return this->pow(_1); }
exprptr_t Expression::pow(Expression &_1, Integer _2)
{
    return this->pow(_1.copyToExprPtr(), _2);
}
exprptr_t Expression::pow(Expression &&_1, Integer _2)
{
    return this->pow(_1.copyToExprPtr(), _2);
}
exprptr_t pow(exprptr_t _1, exprptr_t _2, Integer _3) { return _1->pow(_2, _3); }

exprptr_t Expression::rpow(exprptr_t b)
{
    if (isinstance<True>(this->operator==(Integer(1))))
        return b;
    return std::shared_ptr<Pow>(new Pow({b, this->copyToExprPtr()}));
}

exprptr_t Expression::reciprocal()
{
    static std::vector<std::tuple<exprptr_t>> on_cal; // 正在计算的数据

    exprptr_t self = this->copyToExprPtr();
    exprptr_t __1(new Integer(-1));
    exprptr_t d(new Pow({self, __1}));

    for (auto t : on_cal)
    {
        // 防止递归调用
        if (!isinstance<False>(std::get<0>(t) == self))
            return exprptr_t(new Reciprocal(self));
    }
    on_cal.push_back(std::make_tuple(self));

    exprptr_t t = this->pow(__1);
    if (isinstance<True>(d == t))
        return exprptr_t(new Reciprocal(self));

    on_cal.pop_back();
    return t;
}

exprptr_t Expression::opposite()
{
    static std::vector<std::tuple<exprptr_t>> on_cal; // 正在计算的数据

    exprptr_t self = this->copyToExprPtr();
    exprptr_t __1(new Integer(-1));
    exprptr_t d(new Mul({self, __1}));

    for (auto t : on_cal)
    {
        // 防止递归调用
        if (!isinstance<False>(std::get<0>(t) == self))
            return exprptr_t(new Opposite(self));
    }
    on_cal.push_back(std::make_tuple(self));

    exprptr_t t = this->operator*(__1);
    if (isinstance<True>(d == t))
        return exprptr_t(new Opposite(self));

    on_cal.pop_back();
    return t;
}

exprptr_t Expression::abs()
{
    return exprptr_t(new Abs(this->copyToExprPtr()));
}

objptr_t Expression::operator+(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator+(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator+(b);
}

objptr_t Expression::operator-(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator-(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator-(b);
}

objptr_t Expression::operator*(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator*(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator*(b);
}

objptr_t Expression::operator/(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator/(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator/(b);
}

objptr_t Expression::pow(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->pow(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::pow(b);
}

boolptr_t Expression::operator==(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator==(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator==(b);
}

boolptr_t Expression::operator>(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator>(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator>(b);
}

boolptr_t Expression::operator<(objptr_t b)
{
    if (isinstance<Expression>(b))
        return this->operator<(dynamic_cast<Expression *>(b.get())->copyToExprPtr());
    return Object::operator<(b);
}

/*求导*/
exprptr_t Expression::diff(exprptr_t target)
{
    exprptr_t self = this->copyToExprPtr();

    if (isinstance<True>(this->replace(target,
                                       exprptr_t(new ExprSymbol("_")))
                             ->operator==(self))) // 当成常量
        return exprptr_t(new Integer(0));
    if (isinstance<True>(this->operator==(target)))
        return exprptr_t(new Integer(1));
    return exprptr_t(new Derivative(self, {{target, Integer(1)}}));
}

/*获取系数*/
exprptr_t Expression::getCoef(exprptr_t target)
{
    if (isinstance<True>(this->operator==(target)))
        return exprptr_t(new Integer(1));
    return exprptr_t(new Integer(0));
}

exprptr_t Expression::simplify()
{
    exprptr_t result = this->_simplify();
    exprptr_t pre;
    do
    {
        pre = result;
        result = result->_simplify();
    } while (isinstance<True>(result != pre));

    return result;
}

/*
表达式求值并保留`keep`位有效数字
keep>=0
对于keep<0的情况会将它看成0
*/
exprptr_t Expression::eval(Integer keep)
{
    exprptr_t result = this->simplify()->_eval(keep);
    exprptr_t pre;
    do
    {
        pre = result;
        result = result->simplify()->_eval(keep);
    } while (isinstance<True>(result != pre));

    return result;
}

exprptr_t Expression::_eval(Integer keep)
{
    return exprptr_t(this->simplify()->copyToExprPtr());
}

/*
获取权重
将作为参数排序依据
*/
Integer Expression::getWeight()
{
    return 1;
}