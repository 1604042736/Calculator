#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <memory>

class Boolean;
typedef std::shared_ptr<Boolean> boolptr_t;

class Object;
typedef std::shared_ptr<Object> objptr_t;
typedef std::vector<std::string> prettystring_t;

class Object
{
public:
    virtual std::string toString();
    /*转换成美观的多行字符串*/
    virtual prettystring_t toPrettyString() { return {this->toString()}; }
    /*复制自己*/
    virtual Object *copyThis() = 0;
    /*获取优先级*/
    virtual size_t getPriority() { return -1; }
    /*转换成latex*/
    virtual std::string toLateX() { return this->toString(); }

    virtual objptr_t operator+(objptr_t b);
    friend objptr_t operator+(objptr_t, objptr_t);
    virtual objptr_t operator-(objptr_t b);
    friend objptr_t operator-(objptr_t, objptr_t);
    virtual objptr_t operator*(objptr_t b);
    friend objptr_t operator*(objptr_t, objptr_t);
    virtual objptr_t operator/(objptr_t b);
    friend objptr_t operator/(objptr_t, objptr_t);
    virtual objptr_t pow(objptr_t);

    virtual boolptr_t operator==(objptr_t);
    friend boolptr_t operator==(objptr_t, objptr_t);
    virtual boolptr_t operator!=(objptr_t);
    friend boolptr_t operator!=(objptr_t, objptr_t);

    virtual objptr_t operator()(std::vector<objptr_t>);
    /*替换*/
    virtual objptr_t replace(objptr_t, objptr_t);
    // 判断是否是自己的子类
    virtual bool isSubclass(objptr_t b) { return typeid(*this) == typeid(*b.get()); }
};

prettystring_t normalize(prettystring_t);
void print(prettystring_t);