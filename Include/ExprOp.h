#pragma once

#include <memory>
#include <vector>

#include "Object.h"
#include "Expression.h"

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