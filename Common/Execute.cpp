#include <sstream>

#include "Common.h"
#include "Lexer.h"
#include "Parser.h"
#include "Runtime.h"
#include "Error.h"

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
        exec(code, filename, runtime, verbose);
    }
    catch (Error &e)
    {
        printf("%s:%d:%d 错误: %s\n",
               e.context.filename.data(), e.context.line, e.context.column,
               e.message.data());
    }
    return nullptr;
}