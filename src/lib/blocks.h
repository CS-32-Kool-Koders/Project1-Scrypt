//create AST trees here for evaluation, etc. of statements
#include <iostream>
#include <vector>
#include <string>
// #include "tokens.h"
#include "infix_parser.h"


struct Blocks {
    std::string value;
    
    ExpressionParser *left;
    Blocks *right;
    std::vector<Blocks> blocklist;
    std::vector<Tokens> tokenlist;
    // std::vector<
    
    Blocks(ExpressionParser *left);
    ~Blocks();
    void execute(std::vector<Tokens> tokens);
    

};