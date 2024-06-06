#include "Parser.h"
#include "NameAST.h"
#include "NumAST.h"
#include "BinOpAST.h"
#include "IntAST.h"
#include "FloatAST.h"
#include "Error.h"
#include "CallAST.h"
#include "SymDefAST.h"
#include "CompoundAST.h"
#include "AssignAST.h"
#include "UnaryOpAST.h"
#include "TupleAST.h"
#include "ModifyAST.h"

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
    if (token.type == TK_EOF)
        return nullptr;
    astptr_t a = parse_compound();
    if (token.type != TK_EOF)
        throw Error("解析已结束但文件未结束", lexer->context);
    return a;
}

/*
compound : sentence+
*/
astptr_t Parser::parse_compound()
{
    std::string indent = token.indent;
    std::vector<astptr_t> children({parse_sentence()});
    while (token.type != TK_EOF && token.indent == indent) // 缩进相同才能作为同一个compound
        children.push_back(parse_sentence());
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
    case TK_AT:
        a = parse_modify();
        break;
    case TK_SYM:
        a = parse_symdef();
        break;
    case TK_LET:
        a = parse_let();
        break;
    default:
        a = parse_expr();
    }
    return a;
}

/*
let : 'let' NAME '=' expr
*/
astptr_t Parser::parse_let()
{
    match(token.type, TK_LET);
    std::string name = token.str;
    match(token.type, TK_NAME);
    match(token.type, TK_EQ);
    astptr_t expr = parse_expr();
    return astptr_t(new AssignAST(name, expr, lexer->context));
}

/*
modify : '@' NAME compound
*/
astptr_t Parser::parse_modify()
{
    match(token.type, TK_AT);
    std::string mode = token.str;
    match(token.type, TK_NAME);
    return astptr_t(new ModifyAST(mode, parse_compound(), lexer->context));
}

/*
symdef : 'sym' NAME (',' NAME)*
*/
astptr_t Parser::parse_symdef()
{
    match(token.type, TK_SYM);
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
expr : or_expr
*/
astptr_t Parser::parse_expr()
{
    return parse_or_expr();
}

/*
or_expr : and_expr | and_expr 'or' or_expr
*/
astptr_t Parser::parse_or_expr()
{
    astptr_t l = parse_and_expr();
    while (token.type == TK_OR)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_and_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
and_expr : not_expr | not_expr 'or' and_expr
*/
astptr_t Parser::parse_and_expr()
{
    astptr_t l = parse_not_expr();
    while (token.type == TK_AND)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_not_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
not_expr : compare_expr | 'not' not_expr
*/
astptr_t Parser::parse_not_expr()
{
    astptr_t a;
    if (token.type == TK_NOT)
    {
        match(token.type, TK_NOT);
        a = astptr_t(new UnaryOpAST("not", parse_not_expr(), lexer->context));
    }
    else
        a = parse_compare_expr();
    return a;
}

/*
compare_expr : add_expr | add_expr '=' add_expr | add_expr '!=' add_expr
                        | add_expr '>' add_expr | add_expr '>=' add_expr
                        | add_expr '<' add_expr | add_expr '<=' add_expr
*/
astptr_t Parser::parse_compare_expr()
{
    astptr_t l = parse_add_expr();
    if (token.type == TK_EQ || token.type == TK_NE || token.type == TK_GE || token.type == TK_GT || token.type == TK_LT || token.type == TK_LE)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_add_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
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
mul_expr : prefix_expr | mul_expr '*' prefix_expr | mul_expr '/' prefix_expr
*/
astptr_t Parser::parse_mul_expr()
{
    astptr_t l = parse_prefix_expr();
    while (token.type == TK_MUL || token.type == TK_DIV)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_prefix_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
prefix_expr : pow_expr | '-' prefix_expr
*/
astptr_t Parser::parse_prefix_expr()
{
    astptr_t a;
    switch (token.type)
    {
    case TK_SUB:
        match(token.type, TK_SUB);
        a = astptr_t(new UnaryOpAST("-", parse_prefix_expr(), lexer->context));
        break;

    default:
        a = parse_pow_expr();
    }
    return a;
}

/*
pow_expr : postfix_expr | pow_expr '^' postfix_expr
*/
astptr_t Parser::parse_pow_expr()
{
    astptr_t a = parse_postfix_expr();
    std::vector<astptr_t> children({a});
    while (token.type == TK_POW)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t b = parse_postfix_expr();
        children.push_back(b);
    }
    if (children.size() == 1)
        return children[0];
    astptr_t b = children.back();
    for (size_t i = children.size() - 2; i < children.size(); i--) // 防止溢出
        b = astptr_t(new BinOpAST("^", children[i], b, children[i]->context));
    return b;
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
atom_expr : ID | INT | FLOAT | '(' expr ')' | '(' ( expr ',')+ ')'
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
        if (token.type != TK_RLITTLE)
        {
            a = parse_expr();
            if (token.type == TK_COMMA)
            {
                std::vector<astptr_t> args({a});
                do
                {
                    match(token.type, TK_COMMA);
                    if (token.type == TK_RLITTLE)
                        break;
                    args.push_back(parse_expr());
                } while (token.type == TK_COMMA);
                a = astptr_t(new TupleAST(args, lexer->context));
            }
        }
        match(token.type, TK_RLITTLE);
        break;
    }
    default:
        throw SyntaxError("意料之外的Token: " + token.str, lexer->context);
    }
    return a;
}