#include <sstream>

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
#include "EnumSetAST.h"
#include "FuncDefAST.h"
#include "ScopeAST.h"
#include "ImportAST.h"
#include "Config.h"

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
    case TK_FUNC:
        a = parse_funcdef();
        break;
    case TK_SCOPE:
        a = parse_scope();
        break;
    case TK_IMPORT:
        a = parse_import();
        break;
    default:
        a = parse_expr();
    }
    return a;
}

/*
import : 'import' STRING
*/
astptr_t Parser::parse_import()
{
    match(token.type, TK_IMPORT);
    std::string name = token.str;
    match(token.type, TK_STRING);
    return astptr_t(new ImportAST(name, lexer->context));
}

/*
scope : 'scope' '[' ( NAME ',' )* [NAME] ']' compound
*/
astptr_t Parser::parse_scope()
{
    match(token.type, TK_SCOPE);
    match(token.type, TK_LMIDDLE);
    std::vector<std::string> names;
    while (token.type == TK_NAME)
    {
        names.push_back(token.str);
        match(token.type, TK_NAME);
        if (token.type == TK_COMMA)
            match(token.type, TK_COMMA);
        else
            break;
    }
    match(token.type, TK_RMIDDLE);

    return astptr_t(new ScopeAST(names, parse_compound(), lexer->context));
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
funcdef : 'func' NAME | 'func' NAME '(' NAME (',' NAME)* ')' '=' funcbody
*/
astptr_t Parser::parse_funcdef()
{
    match(token.type, TK_FUNC);
    std::string name = token.str;
    match(token.type, TK_NAME);
    astptr_t a;
    if (token.type != TK_LLITTLE)
        a = astptr_t(new FuncDefAST(name, {}, {}, lexer->context));
    else
    {
        match(token.type, TK_LLITTLE);
        std::vector<std::string> args_name;
        args_name.push_back(token.str);
        match(token.type, TK_NAME);
        while (token.type == TK_COMMA)
        {
            match(token.type, TK_COMMA);
            args_name.push_back(token.str);
            match(token.type, TK_NAME);
        }
        match(token.type, TK_RLITTLE);
        match(token.type, TK_EQ);
        a = astptr_t(new FuncDefAST(name, args_name, parse_funcbody(), lexer->context));
    }
    return a;
}

/*
funcbody : expr [',' expr] ['$' (expr [',' expr])+]
*/
std::vector<funcbodyastptr_t> Parser::parse_funcbody()
{
    std::string indent = token.indent;
    std::vector<funcbodyastptr_t> body;

    while (token.indent == indent)
    {
        astptr_t expr = parse_expr();
        astptr_t domain = nullptr;
        if (token.type == TK_COMMA)
        {
            match(token.type, TK_COMMA);
            domain = parse_expr();
        }
        body.push_back(funcbodyastptr_t(new FuncBodyAST(expr, domain, lexer->context)));
        if (token.type == TK_DOLLAR)
        {
            match(token.type, TK_DOLLAR);
            expr = parse_expr();
            domain = nullptr;
            if (token.type == TK_COMMA)
            {
                match(token.type, TK_COMMA);
                domain = parse_expr();
            }
            body.push_back(funcbodyastptr_t(new FuncBodyAST(expr, domain, lexer->context)));
        }
    }
    return body;
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
compare_expr : bitor_expr | bitor_expr '=' bitor_expr | bitor_expr '!=' bitor_expr
                          | bitor_expr '>' bitor_expr | bitor_expr '>=' bitor_expr
                          | bitor_expr '<' bitor_expr | bitor_expr '<=' bitor_expr
*/
astptr_t Parser::parse_compare_expr()
{
    astptr_t l = parse_bitor_expr();
    if (token.type == TK_EQ || token.type == TK_NE || token.type == TK_GE || token.type == TK_GT || token.type == TK_LT || token.type == TK_LE)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_bitor_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
bitor_expr : bitand_expr | bitor_expr '|' bitand_expr
*/
astptr_t Parser::parse_bitor_expr()
{
    astptr_t l = parse_bitand_expr();
    while (token.type == TK_BITOR)
    {
        std::string op = token.str;
        match(token.type, token.type);
        astptr_t r = parse_bitand_expr();
        l = astptr_t(new BinOpAST(op, l, r, token.context));
    }
    return l;
}

/*
bitand_expr : add_expr | bitand_expr '&' add_expr
*/
astptr_t Parser::parse_bitand_expr()
{
    astptr_t l = parse_add_expr();
    while (token.type == TK_BITAND)
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
    case TK_LLARGE:
        a = parse_enumset();
        break;
    default:
        throw SyntaxError("[parse_atom_expr]意料之外的Token: " + token.str, lexer->context);
    }
    return a;
}

/*
enumset : '{' [ sentence (',' sentence)* ] '}'
*/
astptr_t Parser::parse_enumset()
{
    astptr_t a(new EnumSetAST({}, lexer->context));
    match(token.type, TK_LLARGE);
    if (token.type != TK_RLARGE)
    {
        a->children.push_back(parse_sentence());
        while (token.type == TK_COMMA)
        {
            match(token.type, TK_COMMA);
            if (token.type == TK_RLARGE)
                break;
            a->children.push_back(parse_sentence());
        }
    }
    match(token.type, TK_RLARGE);
    return a;
}

objptr_t exec(std::string code, std::string filename, Runtime *runtime, bool verbose)
{
    Lexer lexer(code, filename);
    Parser parser(&lexer);
    astptr_t ast = parser.parse_start();
    if (ast == nullptr)
        return nullptr;
    if (verbose)
        ast->print();
    objptr_t ret;
    if (runtime == nullptr)
    {
        runtime = new Runtime;
        ret = ast->exec(runtime);
        delete runtime;
    }
    else
        ret = ast->exec(runtime);
    return ret;
}

objptr_t exec(std::string filename, Runtime *runtime, bool verbose, bool run_only)
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("无法打开文件: " + std::string(filename.data()));
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code(buffer.str());
    if (!run_only)
        return exec(code, filename, runtime, verbose);
    try
    {
        simplify(exec(code, filename, runtime, verbose));
    }
    catch (Error &e)
    {
        printf("%s:%d:%d 错误: %s\n",
               e.context.filename.data(), e.context.line, e.context.column,
               e.message.data());
    }
    return nullptr;
}

void shell(Runtime *runtime, bool verbose)
{
    printf("Calculator %d.%d.%d (%s) [%s v%s %s (%s)] on %s\n",
           VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, COMPILE_TIME,
           COMPILER_ID, COMPILER_VERSION, ADDRESS_MODEL,
           SYSTEM_PROCESSOR, SYSTEM_NAME);
    size_t in_count = 1, out_count = 1;
    while (true)
    {
        printf("In [%d]: ", in_count++);
        std::string code;
        std::getline(std::cin, code);
        while (true)
        {
            try
            {
                objptr_t obj = exec(code, "<input>", runtime, verbose);
                if (obj != nullptr)
                {
                    objptr_t s = simplify(obj);
                    if (s != nullptr)
                    {
                        printf("Out [%d]: \n", out_count++);
                        print(s->toPrettyString());
                    }
                }
            }
            catch (TooFewTokenError &)
            {
                std::string more;
                printf("More[%d]:", in_count - 1);
                std::getline(std::cin, more);
                code += "\n" + more;
                continue;
            }
            catch (Error &e)
            {
                printf("%s:%lld:%lld 错误: %s\n",
                       e.context.filename.data(), e.context.line, e.context.column,
                       e.message.data());
            }
            break;
        }
    }
}

void shell(bool verbose)
{
    Runtime runtime;
    shell(&runtime, verbose);
}