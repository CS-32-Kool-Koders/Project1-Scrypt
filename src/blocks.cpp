//ast shit here
#include "lib/blocks.h"
Blocks::Blocks(std::string statement, std::vector<Tokens> tokens, ExpressionParser *left){
    this->value = statement;
    this->tokens = tokens;
    this->left = left;
}
Blocks::~Blocks(){
    delete this->left;
}
void Blocks::execute(std::vector<Tokens> tokens){
    
}