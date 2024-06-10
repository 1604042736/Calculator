#pragma once

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "Context.h"

enum TokenType
{
    TK_NAME,    // 标识符
    TK_INT,     // 整数
    TK_FLOAT,   // 浮点数
    TK_STRING,  // 字符串
    TK_ADD,     // +
    TK_SUB,     // -
    TK_MUL,     // *
    TK_DIV,     // /
    TK_LLITTLE, // (
    TK_RLITTLE, // )
    TK_LMIDDLE, // [
    TK_RMIDDLE, // ]
    TK_LLARGE,  // {
    TK_RLARGE,  // }
    TK_COMMA,   // ,
    TK_POW,     // ^
    TK_EQ,      // =
    TK_NE,      // !=
    TK_GT,      // >
    TK_GE,      // >=
    TK_LT,      // <
    TK_LE,      // <=
    TK_AT,      // @
    TK_BITOR,   // |
    TK_BITAND,  // &
    TK_DOLLAR,  // $
    TK_SYM,
    TK_LET,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_FUNC,
    TK_SCOPE,
    TK_IMPORT,
    TK_EOF
};

class Token
{
public:
    Token() : type(TK_EOF), context("<error>", 0, 0), str("<error>") {}

    Token(TokenType type, Context context, std::string str, std::string indent)
        : type(type), context(context), str(str), indent(indent) {}

    bool operator==(Token b) { return type == b.type && context == b.context; }
    bool operator!=(Token b) { return !this->operator==(b); }

    TokenType type; // 类型
    Context context;
    std::string str;    // token对应的字符串
    std::string indent; // 缩进
};

/*词法分析器*/
class Lexer
{
public:
    Lexer(std::string, std::string);

    char getChar();
    void ungetChar();
    std::string getString(char);
    Token getToken();
    void ungetToken();

    std::vector<Token> tokens;
    size_t cur_token_index;

    std::string code; // 代码
    std::string indent;
    Context context;
    size_t pos; // 当前读到的位置
    bool eof;
};