#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "Rational.h"
#include "Pow.h"
#include "Integer.h"

#include "Infinity.h"

Rational::Rational(Integer nume, Integer deno)
{
    this->nume = nume;
    this->deno = deno;
    this->reduction();
}

Rational::Rational(Float nume, Float deno)
{
    std::tuple<Integer, Integer> t = exponent_matching(nume, deno);
    this->nume = std::get<0>(t);
    this->deno = std::get<1>(t);
    this->reduction();
}

Rational::Rational(int32_t nume, int32_t deno)
{
    this->nume = nume;
    this->deno = deno;
    this->reduction();
}

Rational::Rational(Integer nume)
{
    this->nume = nume;
    this->deno = 1;
}

Rational::Rational(Float nume)
{
    std::tuple<Integer, Integer> t = exponent_matching(nume, 1);
    this->nume = std::get<0>(t);
    this->deno = std::get<1>(t);
    this->reduction();
}

Rational::Rational(double nume)
{
    std::tuple<Integer, Integer> t = exponent_matching(nume, 1);
    this->nume = std::get<0>(t);
    this->deno = std::get<1>(t);
    this->reduction();
}

Rational::Rational(std::string str)
{
    size_t line = str.find('/');
    if (line != std::string::npos)
        new (this) Rational(Float(str.substr(0, line)), Float(str.substr(line + 1)));
    else
        new (this) Rational(Float(str));
}

std::string Rational::toString()
{
    std::stringstream ss;
    ss << this->nume.toString() << '/' << this->deno.toString();
    return ss.str();
}

prettystring_t Rational::toPrettyString()
{
    std::string nume_str = this->nume.toString();
    std::string deno_str = this->deno.toString();

    size_t max_len = std::max(nume_str.size(), deno_str.size());

    std::vector<std::string> result;
    result.push_back(std::string((max_len - nume_str.size()) / 2, ' ') + nume_str);
    result.push_back(std::string(max_len, '-'));
    result.push_back(std::string((max_len - deno_str.size()) / 2, ' ') + deno_str);

    return normalize(result);
}

std::string Rational::toLateX()
{
    return "\\frac{" + this->nume.toLateX() + "}{" + this->deno.toLateX() + "}";
}

void Rational::reduction()
{
    if (this->nume == 0)
    {
        this->deno = 1;
        return;
    }
    Integer a = this->nume, b = this->deno;
    a.sign = b.sign = SIGN_POSITIVE;
    if (a != 0 && b != 0)
    {
        Integer n = std::__gcd(a, b); // 求最大公约数时a,b必须为正
        this->nume = floordiv(this->nume, n);
        this->deno = floordiv(this->deno, n);
    }

    if (this->nume.sign == this->deno.sign)
        this->nume.sign = this->deno.sign = SIGN_POSITIVE;
    else // 将负号移到分子上
        this->nume.sign = SIGN_NEGATIVE, this->deno.sign = SIGN_POSITIVE;
}

Rational Rational::operator+(Rational b)
{
    Rational a(*this);
    return Rational(a.nume * b.deno + b.nume * a.deno, a.deno * b.deno);
}

Rational Rational::operator-(Rational b)
{
    Rational a(*this);
    return Rational(a.nume * b.deno - b.nume * a.deno, a.deno * b.deno);
}

Rational Rational::operator*(Rational b)
{
    Rational a(*this);
    return Rational(a.nume * b.nume, a.deno * b.deno);
}

Rational Rational::operator/(Rational b)
{
    std::swap(b.nume, b.deno); // 取倒数
    return *this * b;
}

Rational operator/(Integer a, Float b) { return Float(a) / b; }

Rational Rational::pow(Integer exp)
{
    if (exp >= 0)
        return Rational(*dynamic_cast<Integer *>(this->nume.pow(exp).get()),
                        *dynamic_cast<Integer *>(this->deno.pow(exp).get()));
    exp.sign = SIGN_POSITIVE;
    return Rational(*dynamic_cast<Integer *>(this->deno.pow(exp).get()),
                    *dynamic_cast<Integer *>(this->nume.pow(exp).get()));
}

Float Rational::pow(Integer exp, Integer keep)
{
    return *dynamic_cast<Float *>(this->pow(exp).eval(keep).get());
}

exprptr_t Rational::operator+(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) + *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this) + *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Rational((*this) + *dynamic_cast<Float *>(_1.get())));
    return Number::operator+(_1);
}

exprptr_t Rational::operator-(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) - *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this) - *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Rational((*this) - *dynamic_cast<Float *>(_1.get())));
    return Number::operator-(_1);
}

exprptr_t Rational::operator*(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) * *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this) * *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Rational((*this) * *dynamic_cast<Float *>(_1.get())));
    return Number::operator*(_1);
}

exprptr_t Rational::operator/(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Float *>(_1.get())));
    return Number::operator/(_1);
}

boolptr_t Rational::operator>(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) > *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) > *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) > *dynamic_cast<Float *>(_1.get()));
    return Number::operator>(_1);
}

boolptr_t Rational::operator<(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) < *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) < *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) < *dynamic_cast<Float *>(_1.get()));
    return Number::operator<(_1);
}

boolptr_t Rational::operator==(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) == *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) == *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) == *dynamic_cast<Float *>(_1.get()));
    return Number::operator==(_1);
}

exprptr_t Rational::pow(exprptr_t _1)
{
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this).pow(*dynamic_cast<Integer *>(_1.get()))));
    return Number::pow(_1);
}

exprptr_t Rational::pow(exprptr_t _1, Integer _2)
{
    if (isinstance<Integer>(_1))
        return exprptr_t(new Float((*this).pow(*dynamic_cast<Integer *>(_1.get()), _2)));
    return Number::pow(_1, _2);
}

exprptr_t Rational::_simplify()
{
    if (this->deno == 0)
        return exprptr_t(new Infinity(this->nume.sign));
    std::vector<Integer> dm = divmod(this->nume, this->deno);
    if (dm[1] == 0)
        return exprptr_t(new Integer(dm[0]));
    return Number::_simplify();
}

exprptr_t Rational::_eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    if (this->deno == 0)
        return exprptr_t(new Infinity());
    return Float(*this, keep).eval(keep);
}