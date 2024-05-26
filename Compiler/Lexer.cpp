#include "Lexer.h"
#include "Error.h"

Lexer::Lexer(std::string code, std::string filename) : context(filename, 1, 0)
{
    this->code = code;

    this->pos = 0;
    this->cur_token_index = 0;
    this->eof = false;
}

char Lexer::getChar()
{
    if (this->pos >= this->code.size())
    {
        this->eof = true;
        return EOF;
    }
    char ch = this->code[this->pos];
    this->context.column++;
    if (ch == ' ' || ch == '\t')
        this->indent += ch;
    else
        this->indent += " ";
    if (this->pos > 0 && this->code[this->pos - 1] == '\n')
    {
        this->context.line++;
        this->indent = "";
    }
    this->pos++;
    return ch;
}

void Lexer::ungetChar()
{
    if (this->eof)
        return;
    this->pos--;
    this->context.column--;
    if (this->code[this->pos] == '\n')
        this->context.line--;
}

Token Lexer::getToken()
{
    if (this->cur_token_index < this->tokens.size())
        return this->tokens[this->cur_token_index++];

    char ch = this->getChar();
    while (ch != EOF && isspace(ch))
        ch = this->getChar();
    if (ch == EOF)
        this->tokens.push_back(Token(TK_EOF, this->context, "<EOF>", indent));
    else if (isalpha(ch) || ch == '_')
    {
        std::string token_str = "";
        do
        {
            token_str += ch;
            ch = this->getChar();
        } while (ch != EOF && isalpha(ch) || ch == '_' || isdigit(ch));
        if (token_str == "expr")
            this->tokens.push_back(Token(KW_EXPR, this->context, token_str, indent));
        else if (token_str == "sym")
            this->tokens.push_back(Token(KW_SYM, this->context, token_str, indent));
        else
            this->tokens.push_back(Token(TK_NAME, this->context, token_str, indent));
        this->ungetChar();
    }
    else if (isdigit(ch))
    {
        std::string token_str = "";
        do
        {
            token_str += ch;
            ch = this->getChar();
        } while (ch != EOF && isdigit(ch));
        if (ch == '.')
        {
            do
            {
                token_str += ch;
                ch = this->getChar();
            } while (ch != EOF && isdigit(ch));
            this->tokens.push_back(Token(TK_FLOAT, this->context, token_str, indent));
        }
        else
            this->tokens.push_back(Token(TK_INT, this->context, token_str, indent));
        this->ungetChar();
    }
    else if (ch == '+')
        this->tokens.push_back(Token(TK_ADD, this->context, "+", indent));
    else if (ch == '-')
        this->tokens.push_back(Token(TK_SUB, this->context, "-", indent));
    else if (ch == '*')
        this->tokens.push_back(Token(TK_MUL, this->context, "*", indent));
    else if (ch == '/')
        this->tokens.push_back(Token(TK_DIV, this->context, "/", indent));
    else if (ch == '(')
        this->tokens.push_back(Token(TK_LLITTLE, this->context, "(", indent));
    else if (ch == ')')
        this->tokens.push_back(Token(TK_RLITTLE, this->context, ")", indent));
    else if (ch == ',')
        this->tokens.push_back(Token(TK_COMMA, this->context, ",", indent));
    else if (ch == '^')
        this->tokens.push_back(Token(TK_POW, this->context, "^", indent));
    else
        throw SyntaxError("Unexpected char: " + ch, this->context);
    return this->tokens[this->cur_token_index++];
}

void Lexer::ungetToken()
{
    if (this->cur_token_index > 0)
        this->cur_token_index--;
}