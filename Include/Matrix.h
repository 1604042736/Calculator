#pragma once

#include "Object.h"
#include "Expression.h"

typedef std::vector<std::vector<exprptr_t>> matrixelements_t;

/*矩阵*/
class Matrix : public Object
{
public:
    Matrix(matrixelements_t);

    virtual Object *copyThis() { return new Matrix(*this); }

    virtual std::string toString();
    virtual prettystring_t toPrettyString();

    virtual Matrix operator+(Matrix);
    virtual Matrix operator*(Matrix);
    virtual Matrix operator*(exprptr_t);

    matrixelements_t elements; // 元素
    size_t rows;               // 行数
    size_t cols;               // 列数
};