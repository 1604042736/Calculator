#include <iostream>

#include "AST.h"

void AST::print(size_t level, std::string indent)
{
    for (size_t i = 0; i < level; i++)
        std::cout << indent;
    std::cout << this->getInfo() << "\n";
    for (size_t i = 0; i < this->children.size(); i++)
        this->children[i]->print(level + 1, indent);
}