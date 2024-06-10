#include <iostream>

#include "Common.h"
#include "Config.h"
#include "Error.h"
#include "Runtime.h"

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
                    printf("Out [%d]: \n", out_count++);
                    print(obj->toPrettyString());
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