
#include <iostream>
#include <vector>
#include <string>
#include "infix_parser.h"

struct Blocks
{
    std::string type;
    ExpressionParser *condition;
    std::string name; // Mainly for functions
    // Blocks *thenBlock;
    // Blocks *elseBlock;
    std::vector<Blocks *>
        blocklist;

    Blocks() : condition(nullptr) {}
    ~Blocks();
};
