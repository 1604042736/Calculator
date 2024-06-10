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
    else if (this->pos != 0) // 第一个字符的缩进应该是空字符串
        this->indent += " ";
    if (this->pos > 0 && this->code[this->pos - 1] == '\n')
    {
        this->context.line++;
        this->context.column = 1;
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
    this->indent = this->indent.substr(0, this->indent.size() - 1);
    // this->pos是当前已经读到的
    if (this->pos >= 1 && this->code[this->pos - 1] == '\n')
    {
        this->context.line--;
        this->context.column = 1;
        /*获取这一整行的缩进以及长度*/
        size_t i = this->pos - 1;
        this->indent = "";
        // 防止size_t 溢出
        while (i < this->code.size() && this->code[i] != '\n')
        {
            char ch = this->code[i];
            if (ch == ' ' || ch == '\t')
                this->indent += ch;
            else
                this->indent += " ";
            i--;
            this->context.column++;
        }
    }
}

/*获取字符串*/
std::string Lexer::getString(char mark)
{
    std::string token_str = "";
    char ch = this->getChar();
    while (ch != EOF && ch != mark)
    {
        token_str += ch;
        ch = this->getChar();
    }
    if (ch == EOF)
        throw Error("字符串未结束", this->context);
    return token_str;
}

Token Lexer::getToken()
{
    if (this->cur_token_index < this->tokens.size())
        return this->tokens[this->cur_token_index++];

    char ch = this->getChar();
    while (ch != EOF && isspace(ch))
        ch = this->getChar();
    std::string indent = this->indent; // 字符串的缩进应该是其第一个字符的缩进
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
        if (token_str == "sym")
            this->tokens.push_back(Token(TK_SYM, this->context, token_str, indent));
        else if (token_str == "let")
            this->tokens.push_back(Token(TK_LET, this->context, token_str, indent));
        else if (token_str == "and")
            this->tokens.push_back(Token(TK_AND, this->context, token_str, indent));
        else if (token_str == "or")
            this->tokens.push_back(Token(TK_OR, this->context, token_str, indent));
        else if (token_str == "not")
            this->tokens.push_back(Token(TK_NOT, this->context, token_str, indent));
        else if (token_str == "func")
            this->tokens.push_back(Token(TK_FUNC, this->context, token_str, indent));
        else if (token_str == "scope")
            this->tokens.push_back(Token(TK_SCOPE, this->context, token_str, indent));
        else if (token_str == "import")
            this->tokens.push_back(Token(TK_IMPORT, this->context, token_str, indent));
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
    else if (ch == '[')
        this->tokens.push_back(Token(TK_LMIDDLE, this->context, "[", indent));
    else if (ch == ']')
        this->tokens.push_back(Token(TK_RMIDDLE, this->context, "]", indent));
    else if (ch == '{')
        this->tokens.push_back(Token(TK_LLARGE, this->context, "{", indent));
    else if (ch == '}')
        this->tokens.push_back(Token(TK_RLARGE, this->context, "}", indent));
    else if (ch == ',')
        this->tokens.push_back(Token(TK_COMMA, this->context, ",", indent));
    else if (ch == '^')
        this->tokens.push_back(Token(TK_POW, this->context, "^", indent));
    else if (ch == '"' || ch == '\'')
        this->tokens.push_back(Token(TK_STRING, this->context, this->getString(ch), indent));
    else if (ch == '@')
        this->tokens.push_back(Token(TK_AT, this->context, "@", indent));
    else if (ch == '$')
    {
        ch = this->getChar();
        if (ch == '"' || ch == '\'')
            this->tokens.push_back(Token(TK_NAME, this->context, this->getString(ch), indent));
        else
        {
            this->ungetChar();
            this->tokens.push_back(Token(TK_DOLLAR, this->context, "$", indent));
        }
    }
    else if (ch == '=')
        this->tokens.push_back(Token(TK_EQ, this->context, "=", indent));
    else if (ch == '!')
    {
        ch = this->getChar();
        if (ch == '=')
            this->tokens.push_back(Token(TK_NE, this->context, "!=", indent));
        else
            throw Error("'!'后缺少'='", this->context);
    }
    else if (ch == '>')
    {
        ch = this->getChar();
        if (ch == '=')
            this->tokens.push_back(Token(TK_GE, this->context, ">=", indent));
        else
        {
            this->ungetChar();
            this->tokens.push_back(Token(TK_GT, this->context, ">", indent));
        }
    }
    else if (ch == '<')
    {
        ch = this->getChar();
        if (ch == '=')
            this->tokens.push_back(Token(TK_LE, this->context, "<=", indent));
        else
        {
            this->ungetChar();
            this->tokens.push_back(Token(TK_LT, this->context, "<", indent));
        }
    }
    else if (ch == '|')
        this->tokens.push_back(Token(TK_BITOR, this->context, "|", indent));
    else if (ch == '&')
        this->tokens.push_back(Token(TK_BITAND, this->context, "&", indent));
    else
        throw SyntaxError("意料之外的字符: " + ch, this->context);
    return this->tokens[this->cur_token_index++];
}

void Lexer::ungetToken()
{
    if (this->cur_token_index > 0)
        this->cur_token_index--;
}