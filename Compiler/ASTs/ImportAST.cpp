#include <tuple>
#include <algorithm>
#include <vector>
#include <unistd.h>

#include "ImportAST.h"
#include "Common.h"
#include "Error.h"

#ifdef _WIN32
namespace Win
{
#include "windows.h"

    void import_lib(std::string filename, Runtime *runtime)
    {
        HMODULE lib = LoadLibrary(filename.data());
        if (lib == NULL)
        {
            DWORD error_code = GetLastError();
            throw std::runtime_error("读取'" + filename + "'错误:" + std::to_string(error_code));
        }
        typedef void(WINAPI * load_func_t)(Runtime *);
        load_func_t load_func = (load_func_t)GetProcAddress(lib, "load");
        if (load_func == NULL)
            throw std::runtime_error("在'" + filename + "'中找不到'load'函数");
        load_func(runtime);
    }
};
using Win::import_lib;
#else
void import_lib(std::string filename, Runtime *runtime)
{
    std::cout << "暂不支持在当前操作系统下加载'" + filename << "'\n";
}
#endif

// 库文件后缀
std::vector<std::string> lib_ext = {".dll"};
// 代码文件后缀
std::vector<std::string> code_ext = {".txt"};

/*获取文件的名称和后缀(包含'.')*/
std::tuple<std::string, std::string> splitext(std::string filename)
{
    size_t index = filename.rfind('.');
    std::string name = filename;
    std::string ext = "";
    if (index != std::string::npos)
    {
        name = filename.substr(0, index);
        ext = filename.substr(index);
    }
    return std::make_tuple(name, ext);
}

void import_code(std::string filename, Runtime *runtime)
{
    exec(filename, runtime);
}

objptr_t ImportAST::exec(Runtime *runtime)
{
    std::string filename = this->name;
    auto t = splitext(filename);
    std::string name = std::get<0>(t);
    std::string ext = std::get<1>(t);
    try
    {
        if (ext == "")
        {
            std::vector<std::string> avai_ext(lib_ext); // 可用的后缀
            avai_ext.insert(avai_ext.end(), code_ext.begin(), code_ext.end());
            for (size_t i = 0; i < avai_ext.size(); i++)
            {
                if (access((name + avai_ext[i]).data(), F_OK) == 0)
                {
                    ext = avai_ext[i];
                    filename = name + ext;
                    break;
                }
            }
        }
        if (std::find(lib_ext.begin(), lib_ext.end(), ext) != lib_ext.end())
            import_lib(filename, runtime);
        else if (std::find(code_ext.begin(), code_ext.end(), ext) != code_ext.end())
            import_code(filename, runtime);
        else
            throw Error("import不支持的文件", this->context);
    }
    catch (std::exception &e)
    {
        throw Error(e.what(), this->context);
    }
    return nullptr;
}