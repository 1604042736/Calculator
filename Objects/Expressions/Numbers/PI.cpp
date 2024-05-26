#include "PI.h"
#include "Integer.h"
#include "Rational.h"
#include "Common.h"

exprptr_t PI::operator+(exprptr_t _1)
{
    if (isinstance<PI>(_1))
        return ((*this) + *(PI *)_1.get()).copyToExprPtr();
    return Number::operator+(_1);
}
exprptr_t PI::operator-(exprptr_t _1)
{
    if (isinstance<PI>(_1))
        return ((*this) - *(PI *)_1.get()).copyToExprPtr();
    return Number::operator-(_1);
}
exprptr_t PI::operator*(exprptr_t _1)
{
    if (isinstance<PI>(_1))
        return ((*this) * *(PI *)_1.get()).copyToExprPtr();
    return Number::operator*(_1);
}
exprptr_t PI::operator/(exprptr_t _1)
{
    if (isinstance<PI>(_1))
        return ((*this) / *(PI *)_1.get()).copyToExprPtr();
    return Number::operator/(_1);
}

boolptr_t PI::operator==(exprptr_t _1)
{
    if (isinstance<PI>(_1))
        return to_boolean((*this) == *(PI *)_1.get());
    return Number::operator==(_1);
}

/*
梅钦公式求pi
参考 https://zhuanlan.zhihu.com/p/114320417
*/
exprptr_t PI::eval(Integer keep)
{
    /*
    // 拉马努金公式
    Float x(0);
    for (Integer k = 0; k < keep; k = k + 1)
    {
        Float a = Float(Float(2).pow(Float("0.5"), keep) * Integer(2) / Integer(9801), keep);
        Float b = factorial(k * 4) * (k * 26390 + 1103);
        Float c = *(Integer *)(factorial(k).pow(4) * Integer(396).pow(k * 4))->eval(keep).get();
        x = Float(x + a * b / c, keep);
    }
    return Rational(1, x).eval(keep);
    */
    if (keep < 0)
        keep = 0;
    Integer n(keep * 2);
    Integer a(1), i(n);
    while (i > 0)
    {
        a = a * 10;
        i = i - 1;
    }
    Integer x1 = a * 4 * 5;
    Integer x2 = a * 239;
    Integer s = 0;
    for (i = 1; i < n;)
    {
        /*
        注意x1的初始值乘了5
        所以算第一项时这里等于 a*4/5
        x2同理
        */
        x1 = floordiv(x1, 25);
        x2 = floordiv(x2, 57121);
        s = s + floordiv(x1 - x2, i);
        i = i + 2;
        // 一次循环算两项
        x1 = floordiv(x1, 25);
        x2 = floordiv(x2, 57121);
        s = s + floordiv(x2 - x1, i); // 注意相减顺序
        i = i + 2;
    }

    return Float(s * 4, 0).eval(keep);
}