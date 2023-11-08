//create AST trees here for evaluation, etc. of statements
#include <iostream>
#include <vector>
#include <string>
// #include "tokens.h"
#include "infix_parser.h"


struct Blocks {
    std::string value;
    ExpressionParser *left;
    // blocks *right;
    std::vector<Tokens> tokens;
    std::vector<std::vector<Tokens>> tokenlist;
    // std::vector<
    
    Blocks(std::string statement, std::vector<Tokens> tokens, ExpressionParser *left);
    ~Blocks();
    void execute(std::vector<Tokens> tokens);

};