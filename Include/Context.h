#pragma once

#include <string>

/*上下文*/
class Context
{
public:
    Context(std::string filename, size_t line, size_t column)
        : filename(filename), line(line), column(column) {}

    bool operator==(Context b) { return filename == b.filename && line == b.line && column == b.column; }

    std::string filename; // 文件名
    size_t line;          // 所在行
    size_t column;        // 所在列
};