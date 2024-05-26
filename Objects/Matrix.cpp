#include <stdexcept>
#include <iostream>

#include "Matrix.h"

Matrix::Matrix(matrixelements_t elements)
{
    this->elements = elements;
    this->rows = this->elements.size();
    if (this->elements.size() == 0)
        this->cols = 0;
    else
        this->cols = this->elements[0].size();
    for (size_t i = 0; i < this->elements.size(); i++)
    {
        if (this->elements[i].size() != this->cols)
            throw std::runtime_error("Illegal Matrix");
    }
}

std::string Matrix::toString()
{
    std::string result = "[";
    for (size_t i = 0; i < this->elements.size(); i++)
    {
        result += "[";
        for (size_t j = 0; j < this->elements[i].size(); j++)
        {
            result += this->elements[i][j]->toString();
            if (j < this->elements[i].size() - 1)
                result += ", ";
        }
        result += "]";
        if (i < this->elements.size() - 1)
            result += ", ";
    }
    result += "]";
    return result;
}

prettystring_t Matrix::toPrettyString()
{
    std::vector<prettystring_t> data_str;
    size_t max_len = 1;                                // 所有元素中最长的
    std::vector<size_t> line_maxheight(this->rows, 0); // 每行中最高的
    for (size_t i = 0; i < this->elements.size(); i++)
    {
        for (size_t j = 0; j < this->elements[i].size(); j++)
        {
            prettystring_t element_str = this->elements[i][j]->toPrettyString();
            data_str.push_back(element_str);
            max_len = std::max(max_len, element_str[0].size());
            line_maxheight[i] = std::max(line_maxheight[i], element_str.size());
        }
    }

    std::vector<prettystring_t> line_str(this->rows); // 矩阵的每一行对应的字符串
    for (size_t i = 0; i < data_str.size(); i++)
    {
        size_t x = i / this->cols;
        if (line_str[x].size() == 0)
        {
            for (size_t j = 0; j < line_maxheight[x]; j++)
                line_str[x].push_back("");
        }
        bool flag = data_str[i][0].size() < max_len;
        size_t d = 0;
        if (flag)
        {
            // 居中对齐
            d = (max_len - data_str[i][0].size() + 1) / 2;
            for (size_t j = 0; j < line_str[x].size(); j++)
                line_str[x][j] += std::string(d, ' ');
        }
        // 与这一行的底部对齐
        for (size_t j = 0; j < line_str[x].size(); j++)
        {
            line_str[x][line_str[x].size() - j - 1] +=
                j + 1 <= data_str[i].size() ? data_str[i][data_str[i].size() - j - 1]
                                            : std::string(data_str[i][0].size(), ' ');
        }
        if (flag)
        {
            for (size_t j = 0; j < line_str[x].size(); j++)
                line_str[x][j] += std::string(d, ' ');
        }
        // 空格, 但会产生多余的一列
        for (size_t j = 0; j < line_str[x].size(); j++)
            line_str[x][j] += " ";
    }
    prettystring_t result;
    for (size_t i = 0; i < line_str.size(); i++)
    {
        prettystring_t line_stri = normalize(line_str[i]); // 删除多余的一列
        for (size_t j = 0; j < line_stri.size(); j++)
            result.push_back(line_stri[j]);
        result.push_back(std::string((max_len + 1) * this->cols - 1, ' ')); //-1是因为有对于的一列存在
    }
    result = normalize(result);

    for (size_t i = 0; i < result.size(); i++)
        result[i] = "[" + result[i] + "]";

    return result;
}

Matrix Matrix::operator+(Matrix b)
{
    if (this->rows != b.rows || this->cols != b.cols)
        throw std::runtime_error("Size mismatch");
    matrixelements_t elements;
    for (size_t i = 0; i < this->rows; i++)
    {
        std::vector<exprptr_t> t;
        for (size_t j = 0; j < this->cols; j++)
            t.push_back(this->elements[i][j] + b.elements[i][j]);
        elements.push_back(t);
    }
    return Matrix(elements);
}

Matrix Matrix::operator*(Matrix b)
{
    if (this->cols != b.rows)
        throw std::runtime_error("Size mismatch");
    matrixelements_t elements(this->rows, std::vector<exprptr_t>(b.cols, nullptr));
    for (size_t i = 0; i < this->rows; i++)
    {
        for (size_t j = 0; j < b.cols; j++)
        {
            for (size_t k = 0; k < this->cols; k++)
            {
                if (elements[i][j] == nullptr)
                    elements[i][j] = this->elements[i][k] * b.elements[k][j];
                else
                    elements[i][j] = elements[i][j] + this->elements[i][k] * b.elements[k][j];
            }
        }
    }
    return Matrix(elements);
}

Matrix Matrix::operator*(exprptr_t b)
{
    matrixelements_t elements(this->elements);
    for (size_t i = 0; i < elements.size(); i++)
    {
        for (size_t j = 0; j < elements[i].size(); j++)
            elements[i][j] = elements[i][j] * b;
    }
    return Matrix(elements);
}