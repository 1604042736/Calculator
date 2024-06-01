#include <iostream>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <fstream>
#include <sstream>

#ifdef _WIN32
namespace Win
{
#include <windows.h>
}
#endif

#include "Integer.h"
#include "Float.h"
#include "Rational.h"
#include "Add.h"
#include "Sqrt.h"
#include "PI.h"
#include "E.h"
#include "ExprSymbol.h"
#include "Boolean.h"
#include "Equality.h"
#include "EnumSet.h"
#include "Lambda.h"
#include "DescribeSet.h"
#include "Abs.h"
#include "Matrix.h"
#include "Parser.h"
#include "Mapping.h"
#include "DefinedFunction.h"
#include "ExprDefFunction.h"
#include "Common.h"
#include "Error.h"
#include "Config.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    Win::SetConsoleOutputCP(CP_UTF8);
#endif

    if (argc == 1)
        shell();
    else
    {
        bool verbose = false;
        std::string filename = "";
        for (int i = 1; i < argc; i++)
        {
            if (std::string(argv[i]) == "--verbose")
                verbose = true;
            else
                filename = std::string(argv[i]);
        }
        if (filename != "")
        {
            std::ifstream file(filename);
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string code(buffer.str());
            try
            {
                exec(code, filename, verbose);
            }
            catch (Error &e)
            {
                printf("%s:%d:%d 错误: %s\n",
                       e.context.filename.data(), e.context.line, e.context.column,
                       e.message.data());
            }
        }
        else
            shell(verbose);
    }
    return 0;
}