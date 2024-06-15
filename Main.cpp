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
using Win::SetConsoleOutputCP;
#endif

#include "Parser.h"
#include "Error.h"

void print_help()
{
    printf("使用: Calculator [选项] [文件...]\n");
    printf("选项:\n");
    printf("--verbose\t输出过程信息\n");
    printf("--shell\t打开shell(直接打开或者运行完文件后打开)\n");
    printf("--help\t输出帮助\n");
    printf("参数:\n");
    printf("文件\t需要解析的一个或多个文件\n");
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
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
            else if (arg == "--help")
            {
                print_help();
                return 0;
            }
            else
                filenames.push_back(arg);
        }
        Runtime runtime;
        if (filenames.size() > 0)
        {
            for (auto filename : filenames)
            {
                try
                {
                    exec(filename, &runtime, verbose, true);
                }
                catch (std::exception &e)
                {
                    std::cout << e.what() << "\n";
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