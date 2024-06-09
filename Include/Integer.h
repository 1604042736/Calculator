#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cmath>
#include <limits>

#include "Object.h"
#include "Expression.h"
#include "IntegerSet.h"
#include "Number.h"

typedef uint32_t section_t;
/*
对于`uint32_t`来说, 它能表示的最大正整数为 4294967296,
所以如果以它为`section_t`的话, 每段最大值是 999999999, 每段的长度是 9
*/
const uint64_t SECTION_LEN = std::log10(std::numeric_limits<section_t>::max());
const uint64_t SECTION_MAX = std::pow(10, SECTION_LEN); // 为了方便计算, 实际上取的是最大值+1

enum Sign
{
    SIGN_POSITIVE = 1,
    SIGN_NEGATIVE = -1
};

/*
每个元素都有三个有效位
从左到右第一位代表大于, 第二位代表等于, 第三位代表小于
某位为1表示相应位代表的关系成立
*/
enum CompareFlag
{
    CF_GT = 0b100,
    CF_EQ = 0b010,
    CF_LT = 0b001,
    CF_GE = 0b110,
    CF_LE = 0b011,
    CF_NE = 0b101
};

class Rational;
class Float;
class Integer;
bool compare(Integer, Integer, CompareFlag);
std::vector<Integer> divmod(Integer, Integer);
Integer floordiv(Integer, Integer);

class Integer : public Number
{
public:
    Integer();
    Integer(std::vector<section_t>, Sign);
    Integer(std::string);
    Integer(uint64_t);
    Integer(int64_t);
    Integer(int32_t);

    void normalize();
    Integer length();

    virtual std::string toString();

    virtual Object *copyThis() { return new Integer(*this); }

    uint64_t toUInt64()
    {
        uint64_t a = 0;
        for (size_t i = this->sections.size() - 1; i < this->sections.size(); i--)
            a = a * SECTION_MAX + this->sections[i];
        return a;
    }

    int64_t toInt64()
    {
        int64_t a = 0;
        for (size_t i = this->sections.size() - 1; i < this->sections.size(); i--)
            a = a * SECTION_MAX + this->sections[i];
        return a * (sign == SIGN_POSITIVE ? 1 : -1);
    }

    virtual double toDouble() { return this->toInt64(); }

    virtual bool operator>(const Integer a) const { return compare(*this, a, CF_GT); }
    virtual bool operator>=(const Integer a) const { return compare(*this, a, CF_GE); }
    virtual bool operator<(const Integer a) const { return compare(*this, a, CF_LT); }
    virtual bool operator<=(const Integer a) const { return compare(*this, a, CF_LE); }
    virtual bool operator==(const Integer a) const { return compare(*this, a, CF_EQ); }
    virtual bool operator!=(const Integer a) const { return compare(*this, a, CF_NE); }

    virtual bool operator>(int a) { return compare(*this, a, CF_GT); }
    virtual bool operator>=(int a) { return compare(*this, a, CF_GE); }
    virtual bool operator<=(int a) { return compare(*this, a, CF_LE); }
    virtual bool operator<(int a) { return compare(*this, a, CF_LT); }
    virtual bool operator==(int a) { return compare(*this, a, CF_EQ); }
    virtual bool operator!=(int a) { return compare(*this, a, CF_NE); }

    virtual Integer operator+(Integer);
    virtual Integer operator-(Integer);
    virtual Integer operator*(Integer);
    virtual Rational operator/(Integer);
    virtual Integer operator%(Integer b) { return divmod(*this, b)[1]; }

    virtual Integer operator+(int b) { return *this + Integer(b); }
    virtual Integer operator-(int b) { return *this - Integer(b); }
    virtual Integer operator*(int b) { return *this * Integer(b); }
    virtual Rational operator/(int b);
    virtual Integer operator%(int b) { return divmod(*this, b)[1]; }

    virtual exprptr_t pow(Integer);
    virtual Float pow(Integer, Integer);

    virtual exprptr_t pow(int n) { return this->pow(Integer(n)); }
    virtual Float pow(int n, Integer keep);

    virtual exprptr_t reciprocal();
    virtual exprptr_t opposite() { return exprptr_t(new Integer(this->sections, this->sign == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE)); }

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator/(exprptr_t);
    virtual boolptr_t operator>(exprptr_t);
    virtual boolptr_t operator<(exprptr_t);
    virtual boolptr_t operator==(exprptr_t);
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t pow(exprptr_t, Integer);
    virtual exprptr_t abs() { return exprptr_t(new Integer(this->sections, SIGN_POSITIVE)); }

    virtual exprptr_t eval(Integer);

    //  分段存储数字, 数组的每一个元素对应一个段, 每段能存储的最大数字由`section_t`决定
    //  保证长度不为0, 并且不存在前导0
    std::vector<section_t> sections;
    Sign sign; // 保证0的符号为正
};

Integer factorial(Integer);