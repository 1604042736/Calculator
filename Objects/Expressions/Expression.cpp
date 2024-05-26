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
#include "Common.h"
#include "Derivative.h"

exprptr_t Expression::operator*(exprptr_t b)
{
    if (!isinstance<Add>(this) && isinstance<Add>(b))
        return b * *this;
    else if (!isinstance<Pow>(this) && isinstance<Pow>(b))
        return b * *this;
    return exprptr_t(new Mul({this->copyToExprPtr(), b}));
}

exprptr_t Expression::operator+(exprptr_t _1) { return std::shared_ptr<Add>(new Add({this->copyToExprPtr(), _1})); }
exprptr_t Expression::operator+(Expression &_1) { return (*this) + (_1.copyToExprPtr()); }
exprptr_t Expression::operator+(Expression &&_1) { return (*this) + (_1.copyToExprPtr()); }
exprptr_t operator+(exprptr_t _1, exprptr_t _2) { return _1->operator+(_2); }
exprptr_t operator+(exprptr_t _1, Expression &_2) { return _1->operator+(_2); }
exprptr_t operator+(exprptr_t _1, Expression &&_2) { return _1->operator+(_2); }
exprptr_t Expression::operator-(exprptr_t _1) { return this->operator+(_1->opposite()); }
exprptr_t Expression::operator-(Expression &_1) { return (*this) - (_1.copyToExprPtr()); }
exprptr_t Expression::operator-(Expression &&_1) { return (*this) - (_1.copyToExprPtr()); }
exprptr_t operator-(exprptr_t _1, exprptr_t _2) { return _1->operator-(_2); }
exprptr_t operator-(exprptr_t _1, Expression &_2) { return _1->operator-(_2); }
exprptr_t operator-(exprptr_t _1, Expression &&_2) { return _1->operator-(_2); }
exprptr_t Expression::operator*(Expression &_1) { return (*this) * (_1.copyToExprPtr()); }
exprptr_t Expression::operator*(Expression &&_1) { return (*this) * (_1.copyToExprPtr()); }
exprptr_t operator*(exprptr_t _1, exprptr_t _2) { return _1->operator*(_2); }
exprptr_t operator*(exprptr_t _1, Expression &_2) { return _1->operator*(_2); }
exprptr_t operator*(exprptr_t _1, Expression &&_2) { return _1->operator*(_2); }
exprptr_t Expression::operator/(exprptr_t _1) { return this->operator*(_1->reciprocal()); }
exprptr_t Expression::operator/(Expression &_1) { return (*this) / (_1.copyToExprPtr()); }
exprptr_t Expression::operator/(Expression &&_1) { return (*this) / (_1.copyToExprPtr()); }
exprptr_t operator/(exprptr_t _1, exprptr_t _2) { return _1->operator/(_2); }
exprptr_t operator/(exprptr_t _1, Expression &_2) { return _1->operator/(_2); }
exprptr_t operator/(exprptr_t _1, Expression &&_2) { return _1->operator/(_2); }
boolptr_t Expression::operator>(exprptr_t _1) { return boolptr_t(new StrictGreaterThan(this->copyToExprPtr(), _1)); }
boolptr_t Expression::operator>(Expression &_1) { return (*this) > (_1.copyToExprPtr()); }
boolptr_t Expression::operator>(Expression &&_1) { return (*this) > (_1.copyToExprPtr()); }
boolptr_t operator>(exprptr_t _1, exprptr_t _2) { return _1->operator>(_2); }
boolptr_t operator>(exprptr_t _1, Expression &_2) { return _1->operator>(_2); }
boolptr_t operator>(exprptr_t _1, Expression &&_2) { return _1->operator>(_2); }
boolptr_t Expression::operator>=(exprptr_t _1) { return (*this == _1) || (*this > _1); }
boolptr_t Expression::operator>=(Expression &_1) { return (*this) >= (_1.copyToExprPtr()); }
boolptr_t Expression::operator>=(Expression &&_1) { return (*this) >= (_1.copyToExprPtr()); }
boolptr_t operator>=(exprptr_t _1, exprptr_t _2) { return _1->operator>=(_2); }
boolptr_t operator>=(exprptr_t _1, Expression &_2) { return _1->operator>=(_2); }
boolptr_t operator>=(exprptr_t _1, Expression &&_2) { return _1->operator>=(_2); }
boolptr_t Expression::operator<(exprptr_t _1)
{
    return boolptr_t(new StrictLessThan(this->copyToExprPtr(), _1));
}
boolptr_t Expression::operator<(Expression &_1) { return (*this) < (_1.copyToExprPtr()); }
boolptr_t Expression::operator<(Expression &&_1) { return (*this) < (_1.copyToExprPtr()); }
boolptr_t operator<(exprptr_t _1, exprptr_t _2) { return _1->operator<(_2); }
boolptr_t operator<(exprptr_t _1, Expression &_2) { return _1->operator<(_2); }
boolptr_t operator<(exprptr_t _1, Expression &&_2) { return _1->operator<(_2); }
boolptr_t Expression::operator<=(exprptr_t _1) { return (*this == _1) || (*this < _1); }
boolptr_t Expression::operator<=(Expression &_1) { return (*this) <= (_1.copyToExprPtr()); }
boolptr_t Expression::operator<=(Expression &&_1) { return (*this) <= (_1.copyToExprPtr()); }
boolptr_t operator<=(exprptr_t _1, exprptr_t _2) { return _1->operator<=(_2); }
boolptr_t operator<=(exprptr_t _1, Expression &_2) { return _1->operator<=(_2); }
boolptr_t operator<=(exprptr_t _1, Expression &&_2) { return _1->operator<=(_2); }
boolptr_t Expression::operator==(exprptr_t _1) { return to_boolean(false); }
boolptr_t Expression::operator==(Expression &_1) { return (*this) == (_1.copyToExprPtr()); }
boolptr_t Expression::operator==(Expression &&_1) { return (*this) == (_1.copyToExprPtr()); }
boolptr_t operator==(exprptr_t _1, exprptr_t _2) { return _1->operator==(_2); }
boolptr_t operator==(exprptr_t _1, Expression &_2) { return _1->operator==(_2); }
boolptr_t operator==(exprptr_t _1, Expression &&_2) { return _1->operator==(_2); }
boolptr_t Expression::operator!=(exprptr_t _1) { return !(*this == _1); }
boolptr_t Expression::operator!=(Expression &_1) { return (*this) != (_1.copyToExprPtr()); }
boolptr_t Expression::operator!=(Expression &&_1) { return (*this) != (_1.copyToExprPtr()); }
boolptr_t operator!=(exprptr_t _1, exprptr_t _2) { return _1->operator!=(_2); }
boolptr_t operator!=(exprptr_t _1, Expression &_2) { return _1->operator!=(_2); }
boolptr_t operator!=(exprptr_t _1, Expression &&_2) { return _1->operator!=(_2); }
exprptr_t Expression::sqrt()
{
    return std::shared_ptr<Sqrt>(new Sqrt(this->copyToExprPtr()));
}
exprptr_t Expression::sqrt(Integer _1) { return this->pow(exprptr_t(new Float("0.5")), _1); }
exprptr_t sqrt(exprptr_t _1, Integer _2) { return _1->sqrt(_2); }
exprptr_t Expression::pow(exprptr_t _1)
{
    return std::shared_ptr<Pow>(new Pow({this->copyToExprPtr(), _1}));
}
exprptr_t Expression::pow(Expression &_1) { return (*this).pow(_1.copyToExprPtr()); }
exprptr_t Expression::pow(Expression &&_1) { return (*this).pow(_1.copyToExprPtr()); }
exprptr_t pow(exprptr_t _1, exprptr_t _2) { return _1->pow(_2); }
exprptr_t Expression::pow(exprptr_t _1, Integer _2) { return this->pow(_1); }
exprptr_t Expression::pow(Expression &_1, Integer _2)
{
    return (*this).pow(_1.copyToExprPtr(), _2);
}
exprptr_t Expression::pow(Expression &&_1, Integer _2)
{
    return (*this).pow(_1.copyToExprPtr(), _2);
}
exprptr_t pow(exprptr_t _1, exprptr_t _2, Integer _3) { return _1->pow(_2, _3); }
exprptr_t Expression::reciprocal()
{
    return std::shared_ptr<Reciprocal>(new Reciprocal(this->copyToExprPtr()));
}
exprptr_t Expression::opposite()
{
    return std::shared_ptr<Opposite>(new Opposite(this->copyToExprPtr()));
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

/*求导*/
exprptr_t Expression::diff(exprptr_t target)
{
    if (isinstance<True>(this->operator==(target)))
        return exprptr_t(new Integer(1));
    return exprptr_t(new Derivative(this->copyToExprPtr(), {{target, Integer(1)}}));
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
    exprptr_t result = this->_eval(keep);
    exprptr_t pre;
    do
    {
        pre = result;
        result = result->_eval(keep);
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