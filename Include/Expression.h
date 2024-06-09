#pragma once

#include "Object.h"
#include "Set.h"
#include "RealSet.h"
#include "Boolean.h"

class Expression;
typedef std::shared_ptr<Expression> exprptr_t;

class Integer;
/*代数表达式*/
class Expression : virtual public Object
{
public:
    virtual exprptr_t operator+(exprptr_t);
    virtual exprptr_t operator+(Expression &);
    virtual exprptr_t operator+(Expression &&);
    friend exprptr_t operator+(exprptr_t, exprptr_t);
    friend exprptr_t operator+(exprptr_t, Expression &);
    friend exprptr_t operator+(exprptr_t, Expression &&);
    virtual exprptr_t operator-(exprptr_t);
    virtual exprptr_t operator-(Expression &);
    virtual exprptr_t operator-(Expression &&);
    friend exprptr_t operator-(exprptr_t, exprptr_t);
    friend exprptr_t operator-(exprptr_t, Expression &);
    friend exprptr_t operator-(exprptr_t, Expression &&);
    virtual exprptr_t operator*(exprptr_t);
    virtual exprptr_t operator*(Expression &);
    virtual exprptr_t operator*(Expression &&);
    friend exprptr_t operator*(exprptr_t, exprptr_t);
    friend exprptr_t operator*(exprptr_t, Expression &);
    friend exprptr_t operator*(exprptr_t, Expression &&);
    virtual exprptr_t operator/(exprptr_t);
    virtual exprptr_t operator/(Expression &);
    virtual exprptr_t operator/(Expression &&);
    friend exprptr_t operator/(exprptr_t, exprptr_t);
    friend exprptr_t operator/(exprptr_t, Expression &);
    friend exprptr_t operator/(exprptr_t, Expression &&);
    virtual boolptr_t operator>(exprptr_t);
    virtual boolptr_t operator>(Expression &);
    virtual boolptr_t operator>(Expression &&);
    friend boolptr_t operator>(exprptr_t, exprptr_t);
    friend boolptr_t operator>(exprptr_t, Expression &);
    friend boolptr_t operator>(exprptr_t, Expression &&);
    virtual boolptr_t operator>=(exprptr_t);
    virtual boolptr_t operator>=(Expression &);
    virtual boolptr_t operator>=(Expression &&);
    friend boolptr_t operator>=(exprptr_t, exprptr_t);
    friend boolptr_t operator>=(exprptr_t, Expression &);
    friend boolptr_t operator>=(exprptr_t, Expression &&);
    virtual boolptr_t operator<(exprptr_t);
    virtual boolptr_t operator<(Expression &);
    virtual boolptr_t operator<(Expression &&);
    friend boolptr_t operator<(exprptr_t, exprptr_t);
    friend boolptr_t operator<(exprptr_t, Expression &);
    friend boolptr_t operator<(exprptr_t, Expression &&);
    virtual boolptr_t operator<=(exprptr_t);
    virtual boolptr_t operator<=(Expression &);
    virtual boolptr_t operator<=(Expression &&);
    friend boolptr_t operator<=(exprptr_t, exprptr_t);
    friend boolptr_t operator<=(exprptr_t, Expression &);
    friend boolptr_t operator<=(exprptr_t, Expression &&);
    virtual boolptr_t operator==(exprptr_t);
    virtual boolptr_t operator==(Expression &);
    virtual boolptr_t operator==(Expression &&);
    friend boolptr_t operator==(exprptr_t, exprptr_t);
    friend boolptr_t operator==(exprptr_t, Expression &);
    friend boolptr_t operator==(exprptr_t, Expression &&);
    virtual boolptr_t operator!=(exprptr_t);
    virtual boolptr_t operator!=(Expression &);
    virtual boolptr_t operator!=(Expression &&);
    friend boolptr_t operator!=(exprptr_t, exprptr_t);
    friend boolptr_t operator!=(exprptr_t, Expression &);
    friend boolptr_t operator!=(exprptr_t, Expression &&);
    virtual exprptr_t sqrt();
    virtual exprptr_t sqrt(Integer);
    friend exprptr_t sqrt(exprptr_t, Integer);
    virtual exprptr_t pow(exprptr_t);
    virtual exprptr_t pow(Expression &);
    virtual exprptr_t pow(Expression &&);
    friend exprptr_t pow(exprptr_t, exprptr_t);
    virtual exprptr_t pow(exprptr_t, Integer);
    virtual exprptr_t pow(Expression &, Integer);
    virtual exprptr_t pow(Expression &&, Integer);
    friend exprptr_t pow(exprptr_t, exprptr_t, Integer);
    virtual exprptr_t reciprocal();
    virtual exprptr_t opposite();
    virtual exprptr_t abs();
    virtual exprptr_t diff(exprptr_t);

    virtual objptr_t operator+(objptr_t);
    virtual objptr_t operator-(objptr_t);
    virtual objptr_t operator*(objptr_t);
    virtual objptr_t operator/(objptr_t);
    virtual objptr_t pow(objptr_t);
    virtual boolptr_t operator==(objptr_t);
    virtual boolptr_t operator>(objptr_t);
    virtual boolptr_t operator<(objptr_t);

    virtual exprptr_t inline copyToExprPtr() { return exprptr_t(dynamic_cast<Expression *>(this->copyThis())); }

    virtual exprptr_t getCoef(exprptr_t);
    /*化简*/
    virtual exprptr_t simplify();
    /*化简一次*/
    virtual exprptr_t _simplify() { return this->copyToExprPtr(); }
    /*求值*/
    virtual exprptr_t eval(Integer);
    /*求值一次*/
    virtual exprptr_t _eval(Integer);

    /*表达式的值属于哪个集合*/
    virtual setptr_t belongto() { return setptr_t(new RealSet()); }

    virtual Integer getWeight();
};