#include <string>
#include <vector>

#include "Rational.h"

int test_initfromstring()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"", "0/1"});
    examples.push_back({"+", "0/1"});
    examples.push_back({"-", "0/1"});

    examples.push_back({"0", "0/1"});
    examples.push_back({"+0", "0/1"});
    examples.push_back({"-0", "0/1"});

    examples.push_back({"0.5", "1/2"});
    examples.push_back({"-0.5", "-1/2"});

    examples.push_back({"0.5/2", "1/4"});

    examples.push_back({"0.5/0.02", "25/1"});

    examples.push_back({"5/10", "1/2"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Rational a(examples[i][0]);
        std::string answer = a.toString();
        if (answer != examples[i][1])
        {
            printf("[test_initfromstring] (%s)=(%s) (期望 '%s')\n", examples[i][0].data(), answer.data(), examples[i][1].data());
            ret = 1;
        }
    }
    return ret;
}

int main()
{
    int ret = 0;
    ret += test_initfromstring();
    return 0;
}