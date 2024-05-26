#pragma once

#include "Lexer.h"
#include "AST.h"

/*语法分析器*/
class Parser
{
public:
    Parser(Lexer *lexer) : lexer(lexer) {}

    void match(TokenType, TokenType);

    astptr_t parse_start();
    astptr_t parse_compound();
    astptr_t parse_sentence();
    astptr_t parse_exprmod();
    astptr_t parse_symdef();
    astptr_t parse_expr();
    astptr_t parse_add_expr();
    astptr_t parse_mul_expr();
    astptr_t parse_pow_expr();
    astptr_t parse_postfix_expr();
    std::vector<astptr_t> parse_argument_list();
    astptr_t parse_atom_expr();

    Lexer *lexer;
    Token token;
};