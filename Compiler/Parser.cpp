#include "Parser.h"
#include "NameAST.h"
#include "NumAST.h"
#include "BinOpAST.h"
#include "IntAST.h"
#include "FloatAST.h"
#include "Error.h"
#include "CallAST.h"
#include "ExprModAST.h"
#include "SymDefAST.h"
#include "CompoundAST.h"

void Parser::match(TokenType cur, TokenType expect)
{
    if (cur != expect)
    {
        if (cur == TK_EOF)
            throw TooFewTokenError(lexer->context);
        throw SyntaxError("期望得到 " + std::to_string(expect) + ", 但实际得到 " + std::to_string(cur),
                          lexer->context);
    }
    this->token = lexer->getToken();
}

/*
start : compound
*/
astptr_t Parser::parse_start()
{
    this->token = lexer->getToken();
    return parse_compound();
}

/*
compound : sentence+
*/
astptr_t Parser::parse_compound()
{
    std::string indent = token.indent;
    std::vector<astptr_t> children({parse_sentence()});
    while (token.type != TK_EOF && token.indent == indent) // 缩进相同才能作为同一个compound
    {
        children.push_back(parse_sentence());
    }
    if (children.size() == 1)
        return children[0];
    return astptr_t(new CompoundAST(children, lexer->context));
}

/*
sentence : exprmod | symdef | expr
*/
astptr_t Parser::parse_sentence()
{
    astptr_t a;
    switch (token.type)
    {
    case TK_EOF:
        a = nullptr;
        break;
    case KW_EXPR:
        a = parse_exprmod();
        break;
    case KW_SYM:
        a = parse_symdef();
        break;
    default:
        a = parse_expr();
    }
    return a;
}

/*
exprmod : 'expr' compound
*/
astptr_t Parser::parse_exprmod()
{
    match(token.type, KW_EXPR);
    return astptr_t(new ExprModAST(parse_compound(), lexer->context));
}

/*
symdef : 'sym' NAME (',' NAME)*
*/
astptr_t Parser::parse_symdef()
{
    match(token.type, KW_SYM);
    std::vector<std::string> names;
    names.push_back(token.str);
    match(token.type, TK_NAME);
    while (token.type == TK_COMMA)
    {
        match(token.type, TK_COMMA);
        names.push_back(token.str);
        match(token.type, TK_NAME);
    }
    return astptr_t(new SymDefAST(names, lexer->context));
}

/*
expr : add_expr
*/
astptr_t Parser::parse_expr()
{
    return parse_add_expr();
}

/*
add_expr : mul_expr | add_expr '+' mul_expr | add_expr '-' mul_expr
*/
astptr_t Parser::parse_add_expr()
{
    astptr_t l = parse_mul_expr();
    while (token.type == TK_ADD || token.type == TK_SUB)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_mul_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
mul_expr : pow_expr | mul_expr '*' pow_expr | mul_expr '/' pow_expr
*/
astptr_t Parser::parse_mul_expr()
{
    astptr_t l = parse_pow_expr();
    while (token.type == TK_MUL || token.type == TK_DIV)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_pow_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
pow_expr : postfix_expr | pow_expr '^' postfix_expr
*/
astptr_t Parser::parse_pow_expr()
{
    astptr_t l = parse_postfix_expr();
    while (token.type == TK_POW)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_postfix_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
postfix_expr : atom_expr | atom_expr '(' argument_list ')'
*/
astptr_t Parser::parse_postfix_expr()
{
    astptr_t a = parse_atom_expr();
    while (token.type == TK_LLITTLE)
    {
        if (token.type == TK_LLITTLE)
        {
            match(token.type, TK_LLITTLE);
            if (token.type == TK_RLITTLE)
                a = astptr_t(new CallAST(a, {}, token.context));
            else
                a = astptr_t(new CallAST(a, parse_argument_list(), token.context));
            match(token.type, TK_RLITTLE);
        }
    }
    return a;
}

/*
argument_list : expr ( ',' expr )*
*/
std::vector<astptr_t> Parser::parse_argument_list()
{
    std::vector<astptr_t> args;
    args.push_back(parse_expr());
    while (token.type == TK_COMMA)
    {
        match(token.type, TK_COMMA);
        args.push_back(parse_expr());
    }
    return args;
}

/*
atom_expr : ID | INT | FLOAT | '(' expr ')'
*/
astptr_t Parser::parse_atom_expr()
{
    astptr_t a;
    switch (token.type)
    {
    case TK_NAME:
        a = astptr_t(new NameAST(token.str, token.context));
        match(token.type, TK_NAME);
        break;
    case TK_INT:
        a = astptr_t(new IntAST(token.str, token.context));
        match(token.type, TK_INT);
        break;
    case TK_FLOAT:
        a = astptr_t(new FloatAST(token.str, token.context));
        match(token.type, TK_FLOAT);
        break;
    case TK_LLITTLE:
    {
        match(token.type, TK_LLITTLE);
        a = parse_expr();
        match(token.type, TK_RLITTLE);
        break;
    }
    default:
        throw SyntaxError("意料之外的Token: " + token.str, lexer->context);
    }
    return a;
}