#pragma once

#include <memory>
#include <vector>

#include "Object.h"
#include "Expression.h"
#include "True.h"
#include "False.h"

// 参数类型
typedef std::vector<exprptr_t> expropargs_t;

/*表达式运算符*/
class ExprOp : public Expression
{
public:
    ExprOp(expropargs_t);

    virtual boolptr_t operator==(exprptr_t);
    using Expression::operator==;

    /*获取运算符字符串*/
    virtual inline std::string getOpStr() = 0;
    /*是否符合交换律*/
    virtual inline bool isCommutative() { return false; }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator+(Expression &b) { return Expression::operator+(b); }
    virtual exprptr_t operator+(Expression &&b) { return Expression::operator+(b); }

    virtual void sortArgs();

    virtual exprptr_t simplify();

    virtual objptr_t replace(objptr_t, objptr_t);

    expropargs_t args; // 参数
};

/*
无序vector判等
全部相同返回1
全部不同或长度不同返回-1
否则返回0
*/
template <typename T>
int eq_disorder_vec(std::vector<T> a, std::vector<T> b)
{
    if (a.size() != b.size())
        return -1;
    bool all_false = true;
    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < b.size(); j++)
        {
            if (!isinstance<False>(a[i] == b[j]))
                all_false = false;
            if (isinstance<True>(a[i] == b[j]))
            {
                b.erase(b.begin() + j);
                break;
            }
        }
    }
    if (all_false)
        return -1;
    if (b.size() == 0)
        return 1;
    return 0;
}