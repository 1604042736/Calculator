#include <cstdlib>

#include "Float.h"
#include "Rational.h"

int test_initfromstring()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"", "0.e+0"});
    examples.push_back({"+", "0.e+0"});
    examples.push_back({"-", "0.e+0"});

    examples.push_back({"0", "0.e+0"});
    examples.push_back({"+0", "0.e+0"});
    examples.push_back({"-0", "0.e+0"});

    examples.push_back({"10", "1.0e+1"});
    examples.push_back({"+10", "1.0e+1"});
    examples.push_back({"-10", "-1.0e+1"});

    examples.push_back({"00010", "1.0e+1"});
    examples.push_back({"+00010", "1.0e+1"});
    examples.push_back({"-00010", "-1.0e+1"});

    examples.push_back({"000123456789", "1.23456789e+8"});
    examples.push_back({"+000123456789", "1.23456789e+8"});
    examples.push_back({"-000123456789", "-1.23456789e+8"});

    examples.push_back({"123456789987654321", "1.23456789987654321e+17"});
    examples.push_back({"+123456789987654321", "1.23456789987654321e+17"});
    examples.push_back({"-123456789987654321", "-1.23456789987654321e+17"});

    examples.push_back({"0.00000123456789987654321", "1.23456789987654321e-6"});
    examples.push_back({"+0.00000123456789987654321", "1.23456789987654321e-6"});
    examples.push_back({"-0.00000123456789987654321", "-1.23456789987654321e-6"});

    examples.push_back({"1.25e+10", "1.25e+10"});
    examples.push_back({"1.25e-10", "1.25e-10"});
    examples.push_back({"-1.25e+10", "-1.25e+10"});
    examples.push_back({"-1.25e-10", "-1.25e-10"});
    examples.push_back({"+1.25e+10", "1.25e+10"});
    examples.push_back({"+1.25e-10", "1.25e-10"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Float a(examples[i][0]);
        std::string answer = a.toString();
        if (answer != examples[i][1])
        {
            printf("[test_initfromstring] (%s)=(%s) (期望 '%s')\n", examples[i][0].data(), answer.data(), examples[i][1].data());
            ret = 1;
        }
    }
    return ret;
}

int test_initfromrational()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"1/7", "16", "1.428571428571428e-1"});
    examples.push_back({"1/2", "16", "5.e-1"});
    examples.push_back({"10/5", "16", "2.e+0"});

    examples.push_back({"1/-7", "16", "-1.428571428571428e-1"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Rational a(examples[i][0]);
        Float b(a, atoi(examples[i][1].data()));
        std::string answer = b.toString();
        if (answer != examples[i][2])
        {
            printf("[test_initfromrational] (%s , 保留 %s 位)=(%s) (期望 '%s')\n", a.toString().data(), examples[i][1].data(), answer.data(), examples[i][2].data());
            ret = 1;
        }
    }

    return ret;
}

int test_add()
{
    int ret = 0;

    std::vector<std::vector<std::string>> examples;
    examples.push_back({"0", "0", "0.e+0"});
    examples.push_back({"1", "1", "2.e+0"});

    examples.push_back({"1", "2", "3.e+0"});
    examples.push_back({"1", "-2", "-1.e+0"});
    examples.push_back({"-1", "2", "1.e+0"});
    examples.push_back({"-1", "-2", "-3.e+0"});

    examples.push_back({"1234567.89", "0.0000123456789", "1.2345678900123456789e+6"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Float a(examples[i][0]);
        Float b(examples[i][1]);
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
    examples.push_back({"0", "0", "0.e+0"});
    examples.push_back({"1", "1", "0.e+0"});

    examples.push_back({"1", "2", "-1.e+0"});
    examples.push_back({"1", "-2", "3.e+0"});
    examples.push_back({"-1", "2", "-3.e+0"});
    examples.push_back({"-1", "-2", "1.e+0"});

    examples.push_back({"1234567.89", "0.0000123456789", "1.2345678899876543211e+6"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Float a(examples[i][0]);
        Float b(examples[i][1]);
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
    examples.push_back({"0", "0", "0.e+0"});
    examples.push_back({"1", "1", "1.e+0"});

    examples.push_back({"1", "2", "2.e+0"});
    examples.push_back({"1", "-2", "-2.e+0"});
    examples.push_back({"-1", "2", "-2.e+0"});
    examples.push_back({"-1", "-2", "2.e+0"});

    examples.push_back({"9", "9", "8.1e+1"});
    examples.push_back({"-9", "9", "-8.1e+1"});
    examples.push_back({"9", "-9", "-8.1e+1"});
    examples.push_back({"-9", "-9", "8.1e+1"});

    examples.push_back({"1234567.89", "0.0000123456789", "1.5241578750190521e+1"});

    for (size_t i = 0; i < examples.size(); i++)
    {
        Float a(examples[i][0]);
        Float b(examples[i][1]);
        std::string answer = (a * b).toString();
        if (answer != examples[i][2])
        {
            printf("[test_mul] (%s)*(%s)=(%s) (期望 '%s')\n", a.toString().data(), b.toString().data(), answer.data(), examples[i][2].data());
            ret = 1;
        }
    }

    return ret;
}

int main()
{
    int ret = 0;
    ret += test_initfromstring();
    ret += test_initfromrational();
    ret += test_add();
    ret += test_sub();
    ret += test_mul();
    return ret;
}