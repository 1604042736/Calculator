#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>

#include "Integer.h"

int test_initfromstring()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"", "0"});
    examples.push_back({"+", "0"});
    examples.push_back({"-", "0"});

    examples.push_back({"0", "0"});
    examples.push_back({"+0", "0"});
    examples.push_back({"-0", "0"});

    examples.push_back({"10", "10"});
    examples.push_back({"+10", "10"});
    examples.push_back({"-10", "-10"});

    examples.push_back({"00010", "10"});
    examples.push_back({"+00010", "10"});
    examples.push_back({"-00010", "-10"});

    examples.push_back({"000123456789", "123456789"});
    examples.push_back({"+000123456789", "123456789"});
    examples.push_back({"-000123456789", "-123456789"});

    examples.push_back({"123456789987654321", "123456789987654321"});
    examples.push_back({"+123456789987654321", "123456789987654321"});
    examples.push_back({"-123456789987654321", "-123456789987654321"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Integer a(examples[i][0]);
        std::string answer = a.toString();
        if (answer != examples[i][1])
        {
            printf("[test_initfromstring] (%s)=(%s) (期望 '%s')\n", examples[i][0].data(), answer.data(), examples[i][1].data());
            ret = 1;
        }
    }
    return ret;
}

int test_add()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"0", "0", "0"});

    examples.push_back({"1", "1", "2"});
    examples.push_back({"1", "-1", "0"});
    examples.push_back({"-1", "1", "0"});
    examples.push_back({"-1", "-1", "-2"});

    examples.push_back({"1234567890", "9876543210", "11111111100"});
    examples.push_back({"1234567890", "-9876543210", "-8641975320"});
    examples.push_back({"-1234567890", "9876543210", "8641975320"});
    examples.push_back({"-1234567890", "-9876543210", "-11111111100"});

    examples.push_back({"123456", "9876543210", "9876666666"});
    examples.push_back({"123456", "-9876543210", "-9876419754"});
    examples.push_back({"-123456", "9876543210", "9876419754"});
    examples.push_back({"-123456", "-9876543210", "-9876666666"});

    examples.push_back({"1234567890", "987654", "1235555544"});
    examples.push_back({"1234567890", "-987654", "1233580236"});
    examples.push_back({"-1234567890", "987654", "-1233580236"});
    examples.push_back({"-1234567890", "-987654", "-1235555544"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Integer a(examples[i][0]);
        Integer b(examples[i][1]);
        std::string answer = (a + b).toString();
        if (answer != examples[i][2])
        {
            printf("[test_add] (%s)+(%s)=(%s) (期望 '%s')\n", a.toString().data(), b.toString().data(), answer.data(), examples[i][2].data());
            ret = 1;
        }
    }

    return ret;
}

int test_sub()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"0", "0", "0"});

    examples.push_back({"1", "1", "0"});
    examples.push_back({"1", "-1", "2"});
    examples.push_back({"-1", "1", "-2"});
    examples.push_back({"-1", "-1", "0"});

    examples.push_back({"1234567890", "9876543210", "-8641975320"});
    examples.push_back({"1234567890", "-9876543210", "11111111100"});
    examples.push_back({"-1234567890", "9876543210", "-11111111100"});
    examples.push_back({"-1234567890", "-9876543210", "8641975320"});

    examples.push_back({"123456", "9876543210", "-9876419754"});
    examples.push_back({"123456", "-9876543210", "9876666666"});
    examples.push_back({"-123456", "9876543210", "-9876666666"});
    examples.push_back({"-123456", "-9876543210", "9876419754"});

    examples.push_back({"1234567890", "987654", "1233580236"});
    examples.push_back({"1234567890", "-987654", "1235555544"});
    examples.push_back({"-1234567890", "987654", "-1235555544"});
    examples.push_back({"-1234567890", "-987654", "-1233580236"});

    examples.push_back({"12345678900000000000", "123456789", "12345678899876543211"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Integer a(examples[i][0]);
        Integer b(examples[i][1]);
        std::string answer = (a - b).toString();
        if (answer != examples[i][2])
        {
            printf("[test_sub] (%s)-(%s)=(%s) (期望 '%s')\n", a.toString().data(), b.toString().data(), answer.data(), examples[i][2].data());
            ret = 1;
        }
    }

    return ret;
}

int test_mul()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"0", "0", "0"});

    examples.push_back({"1", "1", "1"});
    examples.push_back({"-1", "1", "-1"});
    examples.push_back({"1", "-1", "-1"});
    examples.push_back({"-1", "-1", "1"});

    examples.push_back({"9", "9", "81"});
    examples.push_back({"-9", "9", "-81"});
    examples.push_back({"9", "-9", "-81"});
    examples.push_back({"-9", "-9", "81"});

    examples.push_back({"1234567890", "9876543210", "12193263111263526900"});
    examples.push_back({"-1234567890", "9876543210", "-12193263111263526900"});
    examples.push_back({"1234567890", "-9876543210", "-12193263111263526900"});
    examples.push_back({"-1234567890", "-9876543210", "12193263111263526900"});

    examples.push_back({"123456", "9876543210", "1219318518533760"});
    examples.push_back({"-123456", "9876543210", "-1219318518533760"});
    examples.push_back({"123456", "-9876543210", "-1219318518533760"});
    examples.push_back({"-123456", "-9876543210", "1219318518533760"});

    examples.push_back({"1234567890", "987654", "1219325914830060"});
    examples.push_back({"-1234567890", "987654", "-1219325914830060"});
    examples.push_back({"1234567890", "-987654", "-1219325914830060"});
    examples.push_back({"-1234567890", "-987654", "1219325914830060"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Integer a(examples[i][0]);
        Integer b(examples[i][1]);
        std::string answer = (a * b).toString();
        if (answer != examples[i][2])
        {
            printf("[test_mul] (%s)*(%s)=(%s) (期望 '%s')\n", a.toString().data(), b.toString().data(), answer.data(), examples[i][2].data());
            ret = 1;
        }
    }

    return ret;
}

int test_divmod()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"7", "3", "2", "1"});
    examples.push_back({"7", "-3", "-2", "1"});
    examples.push_back({"-7", "3", "-2", "-1"});
    examples.push_back({"-7", "-3", "2", "-1"});

    examples.push_back({"50", "2", "25", "0"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Integer a(examples[i][0]);
        Integer b(examples[i][1]);
        std::vector<Integer> answer = divmod(a, b);
        if (answer[0] != examples[i][2] || answer[1] != examples[i][3])
        {
            printf("[test_divmod] (%s)/(%s)=(%s)...(%s) (期望 '%s', '%s')\n", a.toString().data(), b.toString().data(), answer[0].toString().data(), answer[1].toString().data(), examples[i][2].data(), examples[i][3].data());
            ret = 1;
        }
    }

    return ret;
}

int main()
{
    int ret = 0;
    ret += test_initfromstring();
    ret += test_add();
    ret += test_sub();
    ret += test_mul();
    ret += test_divmod();
    return ret;
}