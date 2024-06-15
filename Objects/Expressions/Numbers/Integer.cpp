#include <sstream>
#include <iomanip>
#include <cstdlib>

#include "Integer.h"
#include "Rational.h"
#include "Pow.h"

Integer::Integer()
{
    this->sections = {0};
    this->sign = SIGN_POSITIVE;
}

Integer::Integer(std::vector<section_t> sections, Sign sign)
{
    this->sections = sections;
    this->sign = sign;
    this->normalize();
}

Integer::Integer(std::string str)
{
    this->sign = SIGN_POSITIVE;
    if (str.size() > 0 && (str[0] == '-' || str[0] == '+'))
    {
        if (str[0] == '-')
            this->sign = SIGN_NEGATIVE;
        str = str.substr(1);
    }
    for (size_t i = 0; str.size() > i * SECTION_LEN; i++)
    {
        std::string section_str;
        if (str.size() < (i + 1) * SECTION_LEN)
            section_str = str.substr(0, str.size() - i * SECTION_LEN);
        else
            section_str = str.substr(str.size() - (i + 1) * SECTION_LEN, SECTION_LEN);
        this->sections.push_back(std::strtoull(section_str.data(), nullptr, 10));
    }
    this->normalize();
}

Integer::Integer(uint64_t num)
{
    this->sign = SIGN_POSITIVE;
    while (num != 0)
    {
        this->sections.push_back(num % SECTION_MAX);
        num /= SECTION_MAX;
    }
    if (this->sections.empty())
        this->sections.push_back(0);
}

Integer::Integer(int64_t num)
{
    new (this) Integer((uint64_t)std::abs(num));
    this->sign = num >= 0 ? SIGN_POSITIVE : SIGN_NEGATIVE;
}

Integer::Integer(int32_t num)
{
    new (this) Integer((int64_t)num);
}

/*
规范化
1. 去除前导0
2. 保证this->sections.size()>0
3. 0的符号为正
*/
void Integer::normalize()
{
    while (this->sections.size() > 0 && this->sections.back() == 0)
        this->sections.erase(this->sections.end() - 1);
    if (this->sections.empty())
        this->sections.push_back(0);
    if (this->sections.size() == 1 && this->sections[0] == 0)
        this->sign = SIGN_POSITIVE;
}

/*计算存储数字的位数*/
Integer Integer::length()
{
    Integer len;
    if (this->sections.size() > 1)
        len = len + Integer(this->sections.size() - 1) * SECTION_LEN;
    if (this->sections.back() != 0) // 最高位不可能为0, 除非这个数就是0 (规范)
        len = len + (uint64_t)std::floor(std::log10(this->sections.back())) + 1;
    else
        len = len + 1;
    return len;
}

std::string Integer::toString()
{
    std::stringstream ss;
    if (this->sign == SIGN_NEGATIVE)
        ss << '-';
    size_t i = this->sections.size() - 1;
    ss << this->sections[i--];
    // 注意i是size_t, 当i=0时, 再减1会溢出, 从而跳出循环
    for (; i < this->sections.size(); i--)
        ss << std::setfill('0') << std::setw(SECTION_LEN) << this->sections[i];
    return ss.str();
}

Integer Integer::operator+(Integer b)
{
    Integer a(*this);
    Sign sign = SIGN_POSITIVE;
    if (a.sign == SIGN_POSITIVE && b.sign == SIGN_NEGATIVE) // a+(-b)=a-b
    {
        a.sign = b.sign = SIGN_POSITIVE;
        return a - b;
    }
    else if (a.sign == SIGN_NEGATIVE && b.sign == SIGN_POSITIVE) //(-a)+b=b-a
    {
        a.sign = b.sign = SIGN_POSITIVE;
        return b - a;
    }
    else if (a.sign == SIGN_NEGATIVE && b.sign == SIGN_NEGATIVE) //(-a)+(-b)=-(a+b)
    {
        a.sign = b.sign = SIGN_POSITIVE;
        sign = SIGN_NEGATIVE;
    }

    std::vector<section_t> sections(std::max(a.sections.size(), b.sections.size()) + 1);
    uint64_t carry = 0; // 进位
    for (size_t i = 0; i < a.sections.size() || i < b.sections.size() || carry != 0; i++)
    {
        uint64_t n1 = i < a.sections.size() ? a.sections[i] : 0;
        uint64_t n2 = i < b.sections.size() ? b.sections[i] : 0;
        uint64_t t = n1 + n2 + carry;
        sections[i] = t % SECTION_MAX;
        carry = t / SECTION_MAX;
    }
    return Integer(sections, sign);
}

Integer Integer::operator-(Integer b)
{
    Integer a(*this);
    if (a.sign == SIGN_POSITIVE && b.sign == SIGN_NEGATIVE) // a-(-b)=a+b
    {
        a.sign = b.sign = SIGN_POSITIVE;
        return a + b;
    }
    else if (a.sign == SIGN_NEGATIVE && b.sign == SIGN_POSITIVE) // (-a)-b=(-a)+(-b)
    {
        a.sign = b.sign = SIGN_NEGATIVE;
        return a + b;
    }
    else if (a.sign == SIGN_NEGATIVE && b.sign == SIGN_NEGATIVE) // (-a)-(-b)=b-a
    {
        a.sign = b.sign = SIGN_POSITIVE;
        std::swap(a, b);
    }

    Sign sign = SIGN_POSITIVE;
    std::vector<section_t> sections(std::max(a.sections.size(), b.sections.size()) + 1);
    uint64_t borrow = 0; // 借位
    if (a < b)
    {
        sign = SIGN_NEGATIVE;
        std::swap(a, b);
    }
    for (size_t i = 0; i < a.sections.size() || i < b.sections.size() || borrow != 0; i++)
    {
        uint64_t n1 = i < a.sections.size() ? a.sections[i] : 0;
        uint64_t n2 = i < b.sections.size() ? b.sections[i] : 0;
        uint64_t t, new_borrow;
        if (n1 < n2 + borrow) // n1 - borrow < n2
            t = SECTION_MAX, new_borrow = 1;
        else
            t = 0, new_borrow = 0;
        t += n1 - n2 - borrow;
        sections[i] = t;
        borrow = new_borrow;
    }
    return Integer(sections, sign);
}

Integer Integer::operator*(Integer b)
{
    Integer a(*this);
    std::vector<section_t> sections(a.sections.size() + b.sections.size(), 0);
    Sign sign = a.sign == b.sign ? SIGN_POSITIVE : SIGN_NEGATIVE;
    a.sign = b.sign = SIGN_POSITIVE;

    for (size_t i = 0; i < a.sections.size(); i++)
    {
        for (size_t j = 0; j < b.sections.size(); j++)
        {
            uint64_t t = sections[i + j] + (uint64_t)a.sections[i] * b.sections[j];
            sections[i + j + 1] += t / SECTION_MAX;
            sections[i + j] = t % SECTION_MAX;
        }
    }
    return Integer(sections, sign);
}

Rational Integer::operator/(Integer b) { return Rational(*this, b); }

Rational Integer::operator/(int b) { return *this / Integer(b); }

exprptr_t Integer::pow(Integer n)
{
    if (n >= 0)
    {
        Integer a(*this), ans(1);
        while (n > 0)
        {
            if (n % 2 == 1)
                ans = ans * a;
            a = a * a;
            n = floordiv(n, 2);
        }
        return exprptr_t(new Integer(ans));
    }
    else // n<0
    {
        n.sign = SIGN_POSITIVE;
        return this->pow(n)->reciprocal();
    }
}

Float Integer::pow(Integer n, Integer keep)
{
    Integer a(n);
    a.sign = SIGN_POSITIVE;
    Integer b = *dynamic_cast<Integer *>(this->pow(a).get()); // 可以保证这里返回的一定是Integer
    if (n >= 0)
        return *dynamic_cast<Float *>(b.eval(keep).get());
    return *dynamic_cast<Float *>(Rational(1, b).eval(keep).get());
}

Float Integer::pow(int n, Integer keep) { return this->pow(Integer(n), keep); }

exprptr_t Integer::reciprocal()
{
    return exprptr_t(new Rational(1, *this));
}

exprptr_t Integer::operator+(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) + *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Integer((*this) + *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this) + *dynamic_cast<Float *>(_1.get())));
    return Number::operator+(_1);
}

exprptr_t Integer::operator-(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) - *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Integer((*this) - *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this) - *dynamic_cast<Float *>(_1.get())));
    return Number::operator-(_1);
}

exprptr_t Integer::operator*(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) * *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Integer((*this) * *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Float((*this) * *dynamic_cast<Float *>(_1.get())));
    return Number::operator*(_1);
}

exprptr_t Integer::operator/(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Rational *>(_1.get())));
    if (isinstance<Integer>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Integer *>(_1.get())));
    if (isinstance<Float>(_1))
        return exprptr_t(new Rational((*this) / *dynamic_cast<Float *>(_1.get())));
    return Number::operator/(_1);
}

boolptr_t Integer::operator>(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) > *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) > *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) > *dynamic_cast<Float *>(_1.get()));
    return Number::operator>(_1);
}

boolptr_t Integer::operator<(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) < *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) < *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) < *dynamic_cast<Float *>(_1.get()));
    return Number::operator<(_1);
}

boolptr_t Integer::operator==(exprptr_t _1)
{
    if (isinstance<Rational>(_1))
        return to_boolean((*this) == *dynamic_cast<Rational *>(_1.get()));
    if (isinstance<Integer>(_1))
        return to_boolean((*this) == *dynamic_cast<Integer *>(_1.get()));
    if (isinstance<Float>(_1))
        return to_boolean((*this) == *dynamic_cast<Float *>(_1.get()));
    return Number::operator==(_1);
}

exprptr_t Integer::pow(exprptr_t _1)
{
    if (isinstance<Integer>(_1))
        return (*this).pow(*dynamic_cast<Integer *>(_1.get()));
    return Number::pow(_1);
}

exprptr_t Integer::pow(exprptr_t _1, Integer _2)
{
    if (isinstance<Integer>(_1))
        return exprptr_t(new Float((*this).pow(*dynamic_cast<Integer *>(_1.get()), _2)));
    else if (isinstance<Float>(_1))
        return Float(*this).pow(_1, _2);
    return Number::pow(_1, _2);
}

exprptr_t Integer::eval(Integer keep)
{
    if (keep < 0)
        keep = 0;
    Integer length = this->length();
    if (keep == length)
        return this->copyToExprPtr();
    else if (keep > length)
    {
        Integer significand(*this);
        while (significand.length() < keep && significand != 0)
            significand = significand * 10;
        return exprptr_t(new Float(significand, length - 1));
    }
    else // if (keep < length)
    {
        Integer significand(*this);
        std::vector<Integer> dm;
        while (significand.length() > keep && significand != 0)
        {
            dm = divmod(significand, 10);
            significand = dm[0];
        }
        if (dm[1] >= 5) // 四舍五入
            significand = significand + 1;
        return exprptr_t(new Float(significand, length - 1));
    }
}

/*如果两数大小关系符合期望则返回true, 否则返回false*/
bool compare(Integer a, Integer b, CompareFlag expectation)
{
    CompareFlag actual;
    if (a.sign == SIGN_POSITIVE && b.sign == SIGN_NEGATIVE)
        actual = CF_GT;
    else if (a.sign == SIGN_NEGATIVE && b.sign == SIGN_POSITIVE)
        actual = CF_LT;
    // 以下比较针对两数绝对值
    else if (a.sections.size() > b.sections.size())
        actual = CF_GT;
    else if (a.sections.size() < b.sections.size())
        actual = CF_LT;
    else if (a.sections.size() == b.sections.size())
    {
        size_t i = a.sections.size() - 1;
        for (; i > 0; i--)
        {
            if (a.sections[i] != b.sections[i])
                break;
        }
        if (a.sections[i] > b.sections[i])
            actual = CF_GT;
        else if (a.sections[i] < b.sections[i])
            actual = CF_LT;
        else if (a.sections[i] == b.sections[i])
            actual = CF_EQ;
    }
done:
    // 负数的大小与其绝对值大小相反(相等除外)
    if (a.sign == SIGN_NEGATIVE && b.sign == SIGN_NEGATIVE)
    {
        if (actual == CF_LT)
            actual = CF_GT;
        else if (actual == CF_GT)
            actual = CF_LT;
    }
    return (actual | expectation) == expectation;
}

/*
两数相除, 返回商和余数 (向0取整)
 7/ 3 =  2 ...  1
-7/ 3 = -2 ... -1
 7/-3 = -2 ...  1
-7/-3 =  2 ... -1
*/
std::vector<Integer> divmod(Integer a, Integer b)
{
    if (b == 0)
        throw std::runtime_error("Division by error");
    std::vector<std::pair<Integer, Integer>> shift_count;

    Integer shift(b), count(1);
    shift.sign = SIGN_POSITIVE;

    Integer quo, rem(a); // 商和余数
    rem.sign = SIGN_POSITIVE;

    do
    {
        shift_count.insert(shift_count.begin(), std::make_pair(shift, count));
        shift = shift * 10;
        count = count * 10;
    } while (shift < rem); // rem == abs(a) 这里只是借用一下

    for (size_t i = 0; i < shift_count.size();)
    {
        rem = rem - shift_count[i].first;
        if (rem.sign == SIGN_NEGATIVE) // 不够减
        {
            rem = rem + shift_count[i].first;
            i++;
        }
        else
            quo = quo + shift_count[i].second;
    }
    quo.sign = a.sign == b.sign ? SIGN_POSITIVE : SIGN_NEGATIVE;
    rem.sign = a.sign;
    return {quo, rem};
}

// 两数相除取整(向0)
Integer floordiv(Integer a, Integer b)
{
    return divmod(a, b)[0];
}

/*
阶乘
无视符号
*/
Integer factorial(Integer a)
{
    a.sign = SIGN_POSITIVE;
    Integer result(1);
    while (a > 0)
        result = result * a;
    return result;
}