#include "Common.h"
#include "Lexer.h"
#include "Parser.h"
#include "Runtime.h"

objptr_t exec(std::string code, std::string filename, Runtime *runtime, bool verbose)
{
    Lexer lexer(code, filename);
    Parser parser(&lexer);
    astptr_t ast = parser.parse_start();
    if (ast == nullptr)
        return nullptr;
    if (verbose)
        ast->print();
    return ast->exec(runtime);
}

objptr_t exec(std::string code, std::string filename, bool verbose)
{
    Runtime runtime;
    return exec(code, filename, &runtime, verbose);
}