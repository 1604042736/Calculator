#pragma once

#include "Lexer.h"
#include "AST.h"

class FuncBodyAST;
/*语法分析器*/
class Parser
{
public:
    Parser(Lexer *lexer) : lexer(lexer) {}

    void match(TokenType, TokenType);

    astptr_t parse_start();
    astptr_t parse_compound();
    astptr_t parse_sentence();
    astptr_t parse_import();
    astptr_t parse_scope();
    astptr_t parse_modify();
    astptr_t parse_symdef();
    astptr_t parse_let();
    astptr_t parse_funcdef();
    std::vector<std::shared_ptr<FuncBodyAST>> parse_funcbody();

    astptr_t parse_expr();
    astptr_t parse_or_expr();
    astptr_t parse_and_expr();
    astptr_t parse_not_expr();
    astptr_t parse_compare_expr();
    astptr_t parse_bitor_expr();
    astptr_t parse_bitand_expr();
    astptr_t parse_add_expr();
    astptr_t parse_mul_expr();
    astptr_t parse_prefix_expr();
    astptr_t parse_pow_expr();
    astptr_t parse_postfix_expr();
    std::vector<astptr_t> parse_argument_list();
    astptr_t parse_atom_expr();
    astptr_t parse_enumset();

    Lexer *lexer;
    Token token;
};