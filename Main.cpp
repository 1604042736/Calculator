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

#include "Common.h"
#include "Error.h"

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
        bool open_shell = false; // 解析文件结束后是否打开shell
        std::vector<std::string> filenames;
        for (int i = 1; i < argc; i++)
        {
            std::string arg(argv[i]);
            if (arg == "--verbose")
                verbose = true;
            else if (arg == "--shell")
                open_shell = true;
            else
                filenames.push_back(arg);
        }
        Runtime runtime;
        if (filenames.size() > 0)
        {
            for (auto filename : filenames)
            {
                std::ifstream file(filename);
                if (!file)
                {
                    printf("无法打开文件: %s\n", filename.data());
                    continue;
                }
                std::stringstream buffer;
                buffer << file.rdbuf();
                std::string code(buffer.str());
                try
                {
                    exec(code, filename, &runtime, verbose);
                }
                catch (Error &e)
                {
                    printf("%s:%d:%d 错误: %s\n",
                           e.context.filename.data(), e.context.line, e.context.column,
                           e.message.data());
                }
            }
        }
        else
            open_shell = true;
        if (open_shell)
            shell(&runtime, verbose);
    }
    return 0;
}