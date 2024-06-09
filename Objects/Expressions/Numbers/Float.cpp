#include <sstream>
#include <string>
#include <iostream>

#include "Float.h"
#include "Rational.h"
#include "Pow.h"
#include "Common.h"
#include "Infinity.h"

Float::Float(Integer significand, Integer exponent)
{
    this->significand = significand;
    this->exponent = exponent;
}

Float::Float(std::string str)
{
    if (str.size() == 0)
    {
        this->exponent = 0;
        this->significand = 0;
        return;
    }
    size_t point_index = str.size(); // 小数点索引
    size_t num_index = str.size();   // 第一个不为0的数的位置
    size_t e_index = str.size();     // 底数的位置
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '.')
            point_index = i;
        else if (num_index == str.size() && '1' <= str[i] && str[i] <= '9')
            num_index = i;
        else if (str[i] == 'e')
            e_index = i;
        if (point_index != str.size() && num_index != str.size() && e_index != str.size())
            break;
    }
    if (e_index != str.size()) // (+/-)A.Xe+Y的形式, 其中1<=A<=9
    {
        this->significand = Integer(str.substr(0, point_index) + str.substr(point_index + 1, e_index - point_index - 1));
        this->exponent = Integer(str.substr(e_index + 1));
    }
    else // (+/-)X.Y的形式
    {
        this->exponent = Integer(point_index) - Integer(num_index);
        if (point_index > num_index)
            this->exponent = this->exponent - 1;
        if (point_index < str.size())
            this->significand = Integer(str.substr(0, point_index) + str.substr(point_index + 1));
        else
            this->significand = Integer(str);
    }
}

Float::Float(Integer num)
{
    this->significand = num;
    this->exponent = num.length() - 1;
}

Float::Float(double num)
{
    std::stringstream ss;
    ss.precision(16);
    ss << num;
    new (this) Float(ss.str());
}

Float::Float(uint64_t num)
{
    this->significand = num;
    if (num != 0)
        this->exponent = (uint64_t)std::floor(std::log10(num));
    else
        this->exponent = 0;
}

Float::Float(int64_t num)
{
    new (this) Float((uint64_t)std::abs(num));
    this->significand.sign = num >= 0 ? SIGN_POSITIVE : SIGN_NEGATIVE;
}

Float::Float(int32_t num)
{
    new (this) Float((int64_t)num);
}

/*
将有理数转换为浮点数
如果除不断, 会保留keep位小数
*/
Float::Float(Rational num, Integer keep)
{
    Integer a = num.nume, b = num.deno;
    Sign sign = a.sign == b.sign ? SIGN_POSITIVE : SIGN_NEGATIVE;
    a.sign = b.sign = SIGN_POSITIVE;
    std::vector<Integer> dm = divmod(a, b);
    this->significand = dm[0];

    bool flag = false; // 标记是确定小数点
    if (this->significand != 0)
    {
        flag = true;
        this->exponent = this->significand.length() - 1;
    }
    for (Integer i = 0; i < keep && dm[1] != 0; i = i + 1)
    {
        dm[1] = dm[1] * 10;
        dm = divmod(dm[1], b);
        this->significand = this->significand * 10 + dm[0];
        if (!flag)
            this->exponent = this->exponent - 1;
        if (dm[0] != 0)
            flag = true;
    }
    this->significand.sign = sign;
}

std::string Float::toString()
{
    std::stringstream ss;
    std::string significand_str = this->significand.toString();
    if (this->significand.sign == SIGN_POSITIVE)
        ss << significand_str[0] << '.' << significand_str.substr(1);
    else if (this->significand.sign = SIGN_NEGATIVE) // 这种情况significand_str[0]应是负号
        ss << significand_str[0] << significand_str[1] << '.' << significand_str.substr(2);
    ss << 'e';
    if (this->exponent.sign == SIGN_POSITIVE)
        ss << '+';
    ss << this->exponent.toString();
    return ss.str();
}

Float Float::operator+(Float b)
{
    Float a(*this);
    std::tuple<Integer, Integer> t = exponent_matching(a, b);
    Integer x = std::get<0>(t), y = std::get<1>(t);

    Integer significand = x + y;
    Integer exponent = std::max(a.exponent, b.exponent);
    return Float(significand, exponent);
}

Float Float::operator-(Float b)
{
    Float a(*this);
    std::tuple<Integer, Integer> t = exponent_matching(a, b);
    Integer x = std::get<0>(t), y = std::get<1>(t);

    Integer significand = x - y;
    Integer exponent = std::max(a.exponent, b.exponent);
    return Float(significand, exponent);
}

Float Float::operator*(Float b)
{
    Float a(*this);
    Integer x = a.significand, y = b.significand;
    Integer significand = x * y;
    /*
    如果x*y没有进位, x*y的长度应该是(x.length() + y.length() - 1), 指数大小就是两数指数大小之和
    否则就应该调整指数
    */
    Integer exponent = a.exponent + b.exponent + (significand.length() - (x.length() + y.length() - 1));
    return Float(significand, exponent);
}

Rational Float::operator/(Float b) { return Rational(*this, b); }

exprptr_t Float::pow(Integer n)
{
    if (*this == 0)
    {
        if (n < 0)
            return exprptr_t(new Infinity());
        else if (n == 0)
            return exprptr_t(new Integer(1));
        else
            return exprptr_t(new Integer(0));
    }
    if (n >= 0)
    {
        Float a(*this), ans(1);
        while (n > 0)
        {
            if (n % 2 == 1)
                ans = ans * a;
            a = a * a;
            n = floordiv(n, 2);
        }
        return exprptr_t(new Float(ans));
    }
    else // n<0
    {
        n.sign = SIGN_POSITIVE;
        return this->pow(n)->reciprocal();
    }
}

Float Float::pow(Float exp, Integer keep)
{
    Float b(exp);
    b.significand.sign = SIGN_POSITIVE;
    Rational fraction(b);
    Rational a = *dynamic_cast<Float *>(this->pow(fraction.nume).get());
    if (exp < 0)
        a = Rational(a.deno, a.nume);
    Integer n = fraction.deno;

    Rational x1(2), k(0);
    while (x1.eval(keep) != k.eval(keep))
    {
        k = x1;
        x1 = (((n - 1) * x1.pow(n) + a) / (n * x1.pow(n - 1)));
    }
    return *dynamic_cast<Float *>(x1.eval(keep).get());
}

Float Float::pow(Integer exp, Integer keep)
{
    Integer a(exp);
    a.sign = SIGN_POSITIVE;
    Float b = *dynamic_cast<Float *>(this->pow(a).get());
    if (exp >= 0)
        return *dynamic_cast<Float *>(b.eval(keep).get());
    return *dynamic_cast<Float *>(Rational(1, b).eval(keep).get());
}

exprptr_t Float::reciprocal()
{
    return exprptr_t(new Rational(1, *this));
}

exprptr_t Float::operator+(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) + *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Float((*this) + *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this) + *dynamic_cast<Float *>(_1.get())));
    return Number::operator+(_1);
}

exprptr_t Float::operator-(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) - *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Float((*this) - *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this) - *dynamic_cast<Float *>(_1.get())));
    return Number::operator-(_1);
}

exprptr_t Float::operator*(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) * *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Float((*this) * *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this) * *dynamic_cast<Float *>(_1.get())));
    return Number::operator*(_1);
}

exprptr_t Float::operator/(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Float *>(_1.get())));
    return Number::operator/(_1);
}

boolptr_t Float::operator>(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) > *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) > *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) > *dynamic_cast<Float *>(_1.get()));
    return Number::operator>(_1);
}

boolptr_t Float::operator<(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) < *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) < *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) < *dynamic_cast<Float *>(_1.get()));
    return Number::operator<(_1);
}

boolptr_t Float::operator==(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) == *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) == *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) == *dynamic_cast<Float *>(_1.get()));
    return Number::operator==(_1);
}

exprptr_t Float::pow(exprptr_t _1)
{
    if (isinstance<Integer>(_1))
        return (*this).pow(*dynamic_cast<Integer *>(_1.get()));
    return Number::pow(_1);
}

exprptr_t Float::pow(exprptr_t _1, Integer _2)
{
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this).pow(*dynamic_cast<Float *>(_1.get()), _2)));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Float((*this).pow(*dynamic_cast<Integer *>(_1.get()), _2)));
    return Number::pow(_1, _2);
}

exprptr_t Float::eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    exprptr_t a = this->significand.eval(keep);
    if (isinstance<Integer>(a)) // 相当于Float的有效位数等于保留位数
        return exprptr_t(new Float(*dynamic_cast<Integer *>(a.get()), this->exponent));
    /*
    314.15926 = 3.1415926e+2
    31415926 (keep 2) = 3.1e+7
    3.1415926e+2 (keep 2) = 3.1e+2
    */
    return exprptr_t(new Float((*dynamic_cast<Float *>(a.get())).significand, this->exponent));
}

bool Float::operator>(Float b)
{
    std::tuple<Integer, Integer> t = exponent_matching(*this, b);
    return std::get<0>(t) > std::get<1>(t);
}

bool Float::operator<(Float b)
{
    std::tuple<Integer, Integer> t = exponent_matching(*this, b);
    return std::get<0>(t) < std::get<1>(t);
}

double Float::toDouble()
{
    double s = this->significand.toDouble();
    double exp = (this->exponent - (this->significand.length() - 1)).toDouble();
    return s * std::pow(10, exp);
}

/*对阶并返回有效位数字(保留各自的符号)*/
std::tuple<Integer, Integer> exponent_matching(Float a, Float b)
{
    //  a和b的小数部分长度
    Integer aflen = a.significand.length() - 1 - a.exponent;
    Integer bflen = b.significand.length() - 1 - b.exponent;
    Integer x = a.significand, y = b.significand;
    // 对齐小数点
    for (; aflen > bflen; bflen = bflen + 1)
        y = y * 10;
    for (; aflen < bflen; aflen = aflen + 1)
        x = x * 10;
    return std::make_tuple(x, y);
}